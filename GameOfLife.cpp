//
// Created by Kuboss on 27.04.2026.
//

#include "GameOfLife.h"
#include "raylib.h"

GameOfLife::GameOfLife(int width, int height, int cellSize):width(width),height(height), cellSize(cellSize) {
    this->currentBoard = std::make_unique<Board>(width, height);
    this->nextBoard = std::make_unique<Board>(width, height);
}
//Dzięki ghostCells nie mam tu ifów, więc mogę zawsze porównywać ze wszystkim naokoło
uint8_t GameOfLife::countAliveNeighbors(int x, int y) {
    return currentBoard->getCell(x + 1, y) + currentBoard->getCell(x + 1, y + 1)
            + currentBoard->getCell(x - 1, y) + currentBoard->getCell(x - 1, y -1)
            + currentBoard->getCell(x - 1, y + 1) + currentBoard->getCell(x + 1, y - 1)
            + currentBoard->getCell(x, y + 1) + currentBoard->getCell(x, y - 1);
}

void GameOfLife::computeNextGeneration() {
    this->synchronizeGhostCells();
    uint8_t state;
    uint8_t neighbors;
    for(int i = 0; i < this->width; i++){
        for(int j = 0; j < this->height; j++){
            state = this->currentBoard->getCell(i,j);
            neighbors = countAliveNeighbors(i,j);
            if(neighbors == 3 && state == 0){
                this->nextBoard->setCell(i, j, 1);
            }else if(state == 1 && (neighbors < 2 || neighbors > 3)){
                this->nextBoard->setCell(i, j, 0);
            }else{
                this->nextBoard->setCell(i, j, state);
            }
        }
    }

    std::swap(this->currentBoard, this->nextBoard);
}

void GameOfLife::drawGeneration() {
    for(int y = 0; y < this->height; y++){
        for(int x = 0; x < this->width; x++){
            if(this->currentBoard->getCell(x, y) == 1){
                DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, BLACK);
            } else {
                DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, WHITE);
            }

            DrawRectangleLines(x * cellSize, y * cellSize, cellSize, cellSize, LIGHTGRAY);
        }
    }
}

//wygenerowany przez Gemini latawiec do testów
void GameOfLife::spawnGlider()
{
    int cx = this->width / 2;
    int cy = this->height / 2;

    this->currentBoard->setCell(cx, cy - 1, 1);
    this->currentBoard->setCell(cx + 1, cy, 1);
    this->currentBoard->setCell(cx - 1, cy + 1, 1);
    this->currentBoard->setCell(cx, cy + 1, 1);
    this->currentBoard->setCell(cx + 1, cy + 1, 1);
}



//funkcja kopiująca krawędzie do ghostcelli - np. kopiuje logicznie ostatni wiersz do fizycznie pierwszego(ghost)
//dzięki temu jest "zawijanie" planszy
void GameOfLife::synchronizeGhostCells()
{
    for(int x = 1; x <= this->width; x++)
    {
        uint8_t bottomRowCell = this->currentBoard->getRawCell(x, this->height);
        this->currentBoard->setRawCell(x, 0, bottomRowCell);

        uint8_t topRowCell = this->currentBoard->getRawCell(x, 1);
        this->currentBoard->setRawCell(x, this->height + 1, topRowCell);
    }

    for(int y = 1; y <= this->height; y++)
    {
        uint8_t rightColCell = this->currentBoard->getRawCell(this->width, y);
        this->currentBoard->setRawCell(0, y, rightColCell);

        uint8_t leftColCell = this->currentBoard->getRawCell(1, y);
        this->currentBoard->setRawCell(this->width + 1, y, leftColCell);
    }


    uint8_t bottomRightCell = this->currentBoard->getRawCell(this->width, this->height);
    this->currentBoard->setRawCell(0, 0, bottomRightCell);

    uint8_t bottomLeftCell = this->currentBoard->getRawCell(1, this->height);
    this->currentBoard->setRawCell(this->width + 1, 0, bottomLeftCell);

    uint8_t topRightCell = this->currentBoard->getRawCell(this->width, 1);
    this->currentBoard->setRawCell(0, this->height + 1, topRightCell);

    uint8_t topLeftCell = this->currentBoard->getRawCell(1, 1);
    this->currentBoard->setRawCell(this->width + 1, this->height + 1, topLeftCell);

}