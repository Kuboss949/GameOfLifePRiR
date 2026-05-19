//
// Created by Kuboss on 27.04.2026.
//

#include "GameOfLifeMPI.h"
#include "raylib.h"
#include <mpi.h>
#include <algorithm>
#include <iostream>

GameOfLifeMPI::GameOfLifeMPI(int globalWidth, int globalHeight, int cellSize, int rank, int size)
        : globalWidth(globalWidth), globalHeight(globalHeight), cellSize(cellSize), rank(rank), size(size)
{
    int baseRows = globalHeight / size;
    int remainder = globalHeight % size;

    // Przydział wierszy
    this->localHeight = baseRows + (rank < remainder ? 1 : 0);
    this->globalStartY = rank * baseRows + std::min(rank, remainder);

    // Alokujemy planszę o szerokości globalnej, ale wysokości tylko lokalnej
    this->currentBoard = std::make_unique<Board>(globalWidth, localHeight);
    this->nextBoard = std::make_unique<Board>(globalWidth, localHeight);
}

uint8_t GameOfLifeMPI::countAliveNeighbors(int x, int y) {
    return currentBoard->getCell(x + 1, y) + currentBoard->getCell(x + 1, y + 1)
           + currentBoard->getCell(x - 1, y) + currentBoard->getCell(x - 1, y - 1)
           + currentBoard->getCell(x - 1, y + 1) + currentBoard->getCell(x + 1, y - 1)
           + currentBoard->getCell(x, y + 1) + currentBoard->getCell(x, y - 1);
}

void GameOfLifeMPI::computeNextGeneration() {
    this->synchronizeGhostCells();
    uint8_t state;
    uint8_t neighbors;

    // Pętla leci po lokalnych wymiarach procesu
    for(int i = 0; i < this->globalWidth; i++){
        for(int j = 0; j < this->localHeight; j++){
            state = this->currentBoard->getCell(i, j);
            neighbors = countAliveNeighbors(i, j);

            if(neighbors == 3 && state == 0){
                this->nextBoard->setCell(i, j, 1);
            } else if(state == 1 && (neighbors < 2 || neighbors > 3)){
                this->nextBoard->setCell(i, j, 0);
            } else {
                this->nextBoard->setCell(i, j, state);
            }
        }
    }

    std::swap(this->currentBoard, this->nextBoard);
}

void GameOfLifeMPI::drawGeneration(const std::vector<uint8_t>& fullBoardBuffer) {
    // Proces 0 rysuje iterując po jednowymiarowym buforze
    for(int y = 0; y < this->globalHeight; y++){
        for(int x = 0; x < this->globalWidth; x++){
            if(fullBoardBuffer[y * globalWidth + x] == 1){
                DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, BLACK);
            } else {
                DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, WHITE);
            }
            DrawRectangleLines(x * cellSize, y * cellSize, cellSize, cellSize, LIGHTGRAY);
        }
    }
}

void GameOfLifeMPI::spawnGlider() {
    int cx = this->globalWidth / 2;
    int cy = this->globalHeight / 2;

    // Funkcja lambda upewniająca się, że dany proces ustawi komórkę tylko wtedy, gdy leży w jego przedziale
    auto placeLocal = [&](int globalX, int globalY) {
        if (globalY >= this->globalStartY && globalY < this->globalStartY + this->localHeight) {
            int localY = globalY - this->globalStartY;
            this->currentBoard->setCell(globalX, localY, 1);
        }
    };

    placeLocal(cx, cy - 1);
    placeLocal(cx + 1, cy);
    placeLocal(cx - 1, cy + 1);
    placeLocal(cx, cy + 1);
    placeLocal(cx + 1, cy + 1);
}

void GameOfLifeMPI::synchronizeGhostCells() {
    // 1. Zawijanie poziome (odbywa się w 100% lokalnie w pamięci każdego procesu)
    for(int y = 1; y <= this->localHeight; y++) {
        uint8_t rightColCell = this->currentBoard->getRawCell(this->globalWidth, y);
        this->currentBoard->setRawCell(0, y, rightColCell);

        uint8_t leftColCell = this->currentBoard->getRawCell(1, y);
        this->currentBoard->setRawCell(this->globalWidth + 1, y, leftColCell);
    }

    // 2. Wymiana pionowa (MPI) - przesyłanie pełnych wierszy włącznie z ghost cellami z lewej i prawej
    int top_neighbor = (rank - 1 + size) % size;
    int bottom_neighbor = (rank + 1) % size;
    int rowBytes = this->globalWidth + 2;

    // Przesłanie dolnego wiersza do sąsiada niżej, odbiór od sąsiada wyżej (do górnego ghost row)
    MPI_Sendrecv(
            this->currentBoard->getRowPointer(this->localHeight), rowBytes, MPI_UINT8_T, bottom_neighbor, 0,
            this->currentBoard->getRowPointer(0), rowBytes, MPI_UINT8_T, top_neighbor, 0,
            MPI_COMM_WORLD, MPI_STATUS_IGNORE
    );

    // Przesłanie górnego wiersza do sąsiada wyżej, odbiór od sąsiada niżej (do dolnego ghost row)
    MPI_Sendrecv(
            this->currentBoard->getRowPointer(1), rowBytes, MPI_UINT8_T, top_neighbor, 1,
            this->currentBoard->getRowPointer(this->localHeight + 1), rowBytes, MPI_UINT8_T, bottom_neighbor, 1,
            MPI_COMM_WORLD, MPI_STATUS_IGNORE
    );

    // Narożniki są automatycznie obsługiwane, ponieważ najpierw wykonano zawijanie poziome, 
    // a MPI_Sendrecv kopiuje wiersze o długości width + 2.
}

void GameOfLifeMPI::gatherBoardToMaster(std::vector<uint8_t>& fullBoardBuffer) {
    std::vector<int> recvCounts(size);
    std::vector<int> displacements(size);

    if (rank == 0) {
        fullBoardBuffer.resize(globalWidth * globalHeight);
        int baseRows = globalHeight / size;
        int remainder = globalHeight % size;

        for (int i = 0; i < size; ++i) {
            int rowsForProc = baseRows + (i < remainder ? 1 : 0);
            recvCounts[i] = rowsForProc * globalWidth;

            int startY = i * baseRows + std::min(i, remainder);
            displacements[i] = startY * globalWidth;
        }
    }

    // Pakujemy czyste dane (bez ghost celli z lewej i prawej strony) do płaskiego bufora, żeby wysłać je do MPI
    std::vector<uint8_t> sendBuffer(globalWidth * localHeight);
    for (int y = 0; y < localHeight; ++y) {
        for (int x = 0; x < globalWidth; ++x) {
            sendBuffer[y * globalWidth + x] = this->currentBoard->getCell(x, y);
        }
    }

    MPI_Gatherv(
            sendBuffer.data(), sendBuffer.size(), MPI_UINT8_T,
            fullBoardBuffer.data(), recvCounts.data(), displacements.data(), MPI_UINT8_T,
            0, MPI_COMM_WORLD
    );
}