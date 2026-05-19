//
// Created by Kuboss on 27.04.2026.
//

#ifndef UNTITLED8_BOARD_H
#define UNTITLED8_BOARD_H


#include <vector>

class Board {
private:
    //width i heght są wymiarami logicznymi, stride jest szerokością fizyczną (uwzględnia ghost cells)
    int width;
    int height;
    int stride;
    std::vector<uint8_t> grid;

public:
    Board(int w, int h);
    //getter dla komórki logicznej
    inline uint8_t getCell(int x, int y) const
    {
        int physX = x + 1;
        int physY = y + 1;
        return grid[physY * stride + physX];
    }
    //setter dla komórki logicznej
    inline void setCell(int x, int y, uint8_t state)
    {
        int physX = x + 1;
        int physY = y + 1;
        grid[physY * stride + physX] = state;
    }
    //getter dla komórki fizycznej
    inline uint8_t getRawCell(int x, int y) const
    {
        return grid[y * stride + x];
    }
    //setter dla komórki fizycznej
    inline void setRawCell(int x, int y, uint8_t state)
    {
        grid[y * stride + x] = state;
    }
    inline uint8_t* getRowPointer(int y) {
        return grid.data() + y * stride;
    }

    uint8_t* data();
};


#endif //UNTITLED8_BOARD_H
