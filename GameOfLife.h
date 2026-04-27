//
// Created by Kuboss on 27.04.2026.
//

#ifndef UNTITLED8_GAMEOFLIFE_H
#define UNTITLED8_GAMEOFLIFE_H

#include <memory>
#include "board.h"

class GameOfLife {
private:
    std::unique_ptr<Board> currentBoard;
    std::unique_ptr<Board> nextBoard;
    int width;
    int height;
    int cellSize;
public:
    GameOfLife(int width, int height, int cellSize);
    uint8_t  countAliveNeighbors(int x, int y);
    void computeNextGeneration();
    void drawGeneration();
    void spawnGlider();
    void synchronizeGhostCells();
};


#endif //UNTITLED8_GAMEOFLIFE_H
