#include "GameOfLifeOMP.h"
#include "raylib.h"
#include <omp.h>

Board::Board(int w, int h) : width(w), height(h), data(w*h, 0) {}

uint8_t Board::getCell(int x, int y) const {
    x = (x + width) % width;
    y = (y + height) % height;
    return data[y * width + x];
}

void Board::setCell(int x, int y, uint8_t val) {
    if (x>=0 && x<width && y>=0 && y<height)
        data[y * width + x] = val;
}

GameOfLifeOMP::GameOfLifeOMP(int w, int h, int cellSize)
    : width(w), height(h), cellSize(cellSize)
{
    currentBoard = std::make_unique<Board>(width, height);
    nextBoard = std::make_unique<Board>(width, height);
}

uint8_t GameOfLifeOMP::countAliveNeighbors(int x, int y) {
    return currentBoard->getCell(x + 1, y) + currentBoard->getCell(x + 1, y + 1)
         + currentBoard->getCell(x - 1, y) + currentBoard->getCell(x - 1, y - 1)
         + currentBoard->getCell(x - 1, y + 1) + currentBoard->getCell(x + 1, y - 1)
         + currentBoard->getCell(x, y + 1) + currentBoard->getCell(x, y - 1);
}

void GameOfLifeOMP::computeNextGeneration() {
#pragma omp parallel for collapse(2)
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            uint8_t state = currentBoard->getCell(i, j);
            uint8_t neighbors = countAliveNeighbors(i, j);

            if(neighbors == 3 && state == 0){
                nextBoard->setCell(i, j, 1);
            } else if(state == 1 && (neighbors < 2 || neighbors > 3)){
                nextBoard->setCell(i, j, 0);
            } else {
                nextBoard->setCell(i, j, state);
            }
        }
    }
    std::swap(currentBoard, nextBoard);
}

void GameOfLifeOMP::spawnGlider() {
    int cx = width / 2;
    int cy = height / 2;

    currentBoard->setCell(cx, cy - 1, 1);
    currentBoard->setCell(cx + 1, cy, 1);
    currentBoard->setCell(cx - 1, cy + 1, 1);
    currentBoard->setCell(cx, cy + 1, 1);
    currentBoard->setCell(cx + 1, cy + 1, 1);
}

void GameOfLifeOMP::drawGeneration() {
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            if(currentBoard->getCell(x, y)){
                DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, BLACK);
            }
        }
    }
}
