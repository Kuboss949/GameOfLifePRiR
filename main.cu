#include <iostream>
#include <vector>
#include <cuda_runtime.h>
#include "raylib.h"

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

int main()
{
    const int width = 150;
    const int height = 100;
    const int cellSize = 10;

    size_t size = width * height * sizeof(uint8_t);

    std::vector<uint8_t> hostBoard(width * height, 0);

    // glider
    int cx = width / 2;
    int cy = height / 2;
    hostBoard[(cy-1)*width + cx] = 1;
    hostBoard[(cy)*width + cx+1] = 1;
    hostBoard[(cy+1)*width + cx-1] = 1;
    hostBoard[(cy+1)*width + cx] = 1;
    hostBoard[(cy+1)*width + cx+1] = 1;

    uint8_t *d_current, *d_next;
    cudaMalloc(&d_current, size);
    cudaMalloc(&d_next, size);

    InitWindow(width * cellSize, height * cellSize, "Game of Life CUDA");
    SetTargetFPS(15);

    dim3 blockSize(16,16);
    dim3 gridSize((width + 15)/16, (height + 15)/16);

    while (!WindowShouldClose())
    {
        cudaMemcpy(d_current, hostBoard.data(), size, cudaMemcpyHostToDevice);

        gameOfLifeKernel<<<gridSize, blockSize>>>(d_current, d_next, width, height);

        cudaMemcpy(hostBoard.data(), d_next, size, cudaMemcpyDeviceToHost);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        for(int y=0;y<height;y++){
            for(int x=0;x<width;x++){
                if(hostBoard[y*width+x]){
                    DrawRectangle(x*cellSize,y*cellSize,cellSize,cellSize,BLACK);
                }
            }
        }

        EndDrawing();
    }

    CloseWindow();

    cudaFree(d_current);
    cudaFree(d_next);

    return 0;
}
