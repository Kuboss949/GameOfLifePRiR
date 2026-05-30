#pragma once
#include <cstdint>

// Kernel (może być inline lub zadeklarowany tutaj)
__global__
void gameOfLifeKernel(uint8_t* current, uint8_t* next, int width, int height);

// API do benchmarku / użycia w innych projektach
void runGameOfLifeStep(
    uint8_t* hostBoard,
    int width,
    int height
);

// wersja bardziej kontrolowana (przydatna do benchmarku)
void runGameOfLifeStepCUDA(
    uint8_t* d_current,
    uint8_t* d_next,
    int width,
    int height
);