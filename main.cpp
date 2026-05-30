#include <iostream>
#include <vector>
#include "raylib.h"
#include "GameOfLifeCUDA.h"

int main()
{
    const int width = 150;
    const int height = 100;
    const int cellSize = 10;

    std::vector<uint8_t> hostBoard(width * height, 0);

    // glider
    int cx = width / 2;
    int cy = height / 2;
    hostBoard[(cy-1)*width + cx] = 1;
    hostBoard[(cy)*width + cx+1] = 1;
    hostBoard[(cy+1)*width + cx-1] = 1;
    hostBoard[(cy+1)*width + cx] = 1;
    hostBoard[(cy+1)*width + cx+1] = 1;

    InitWindow(width * cellSize, height * cellSize, "Game of Life CUDA");
    SetTargetFPS(15);

    while (!WindowShouldClose())
    {
        // ✅ tylko jedno wywołanie
        runGameOfLifeStep(hostBoard.data(), width, height);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        for(int y = 0; y < height; y++){
            for(int x = 0; x < width; x++){
                if(hostBoard[y * width + x]){
                    DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, BLACK);
                }
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}