#pragma once
#include <memory>
#include <vector>
#include <cstdint>

class Board {
public:
    Board(int w, int h);
    uint8_t getCell(int x, int y) const;
    void setCell(int x, int y, uint8_t val);

private:
    int width, height;
    std::vector<uint8_t> data;
};

class GameOfLifeOMP {
public:
    GameOfLifeOMP(int width, int height, int cellSize);

    void computeNextGeneration();
    void drawGeneration();
    void spawnGlider();

private:
    uint8_t countAliveNeighbors(int x, int y);

    int width, height, cellSize;
    std::unique_ptr<Board> currentBoard;
    std::unique_ptr<Board> nextBoard;
};
