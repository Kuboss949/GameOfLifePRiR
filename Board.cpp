//
// Created by Kuboss on 27.04.2026.
//

#include "Board.h"

Board::Board(int w, int h): width(w), height(h), stride(w + 2), grid((w + 2) * (h + 2), 0) {}


uint8_t* Board::data() {
    return grid.data();
}

