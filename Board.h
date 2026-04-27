//
// Created by Kuboss on 27.04.2026.
//

#ifndef UNTITLED8_BOARD_H
#define UNTITLED8_BOARD_H


#include <vector>

class Board {
private:
    int width;
    int height;
    int stride;
    std::vector<uint8_t> grid;

public:
    Board(int w, int h);
    inline uint8_t getCell(int x, int y) const
    {
        int physX = x + 1;
        int physY = y + 1;
        return grid[physY * stride + physX];
    }

    inline void setCell(int x, int y, uint8_t state)
    {
        int physX = x + 1;
        int physY = y + 1;
        grid[physY * stride + physX] = state;
    }

    inline uint8_t getRawCell(int x, int y) const
    {
        return grid[y * stride + x];
    }

    inline void setRawCell(int x, int y, uint8_t state)
    {
        grid[y * stride + x] = state;
    }
    uint8_t* data();
};


#endif //UNTITLED8_BOARD_H
