//
// Created by Kuboss on 27.04.2026.
//

#ifndef UNTITLED8_GAMEOFLIFEMPI_H
#define UNTITLED8_GAMEOFLIFEMPI_H

#include "Board.h"
#include <memory>
#include <cstdint>
#include <vector>

class GameOfLifeMPI {
private:
    int globalWidth;
    int globalHeight;
    int cellSize;

    // Zmienne MPI
    int rank;
    int size;
    int localHeight;
    int globalStartY;

    std::unique_ptr<Board> currentBoard;
    std::unique_ptr<Board> nextBoard;

public:
    GameOfLifeMPI(int globalWidth, int globalHeight, int cellSize, int rank, int size);

    uint8_t countAliveNeighbors(int x, int y);
    void computeNextGeneration();

    // Rysuje planszę na podstawie zbiorczego bufora (tylko na procesie 0)
    void drawGeneration(const std::vector<uint8_t>& fullBoardBuffer);

    void spawnGlider();
    void synchronizeGhostCells();

    // Metoda zbierająca fragmenty od wszystkich procesów
    void gatherBoardToMaster(std::vector<uint8_t>& fullBoardBuffer);
};


#endif //UNTITLED8_GAMEOFLIFE_H
