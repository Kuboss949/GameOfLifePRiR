#include "GameOfLifeCUDA.h"
#include <cuda_runtime.h>
#include <iostream>

__global__
void gameOfLifeKernel(uint8_t* current, uint8_t* next, int width, int height)
{
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if (x >= width || y >= height) return;

    int neighbors = 0;

    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 && dy == 0) continue;

            int nx = (x + dx + width) % width;
            int ny = (y + dy + height) % height;

            neighbors += current[ny * width + nx];
        }
    }

    int idx = y * width + x;

    if (current[idx] == 1) {
        next[idx] = (neighbors == 2 || neighbors == 3);
    } else {
        next[idx] = (neighbors == 3);
    }
}

void runGameOfLifeStepCUDA(
    uint8_t* d_current,
    uint8_t* d_next,
    int width,
    int height
)
{
    dim3 blockSize(16,16);
    dim3 gridSize((width + 15)/16, (height + 15)/16);

    gameOfLifeKernel<<<gridSize, blockSize>>>(d_current, d_next, width, height);
}

void runGameOfLifeStep(
    uint8_t* hostBoard,
    int width,
    int height
)
{
    size_t size = width * height * sizeof(uint8_t);

    uint8_t *d_current, *d_next;

    cudaMalloc(&d_current, size);
    cudaMalloc(&d_next, size);

    cudaMemcpy(d_current, hostBoard, size, cudaMemcpyHostToDevice);

    runGameOfLifeStepCUDA(d_current, d_next, width, height);

    cudaMemcpy(hostBoard, d_next, size, cudaMemcpyDeviceToHost);

    cudaFree(d_current);
    cudaFree(d_next);
}