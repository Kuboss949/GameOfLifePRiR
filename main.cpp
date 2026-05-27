#include "GameOfLifeOMP.h"
#include "raylib.h"

int main() {
    int width = 150;
    int height = 100;
    int cellSize = 10;

    GameOfLifeOMP game(width, height, cellSize);
    game.spawnGlider();

    InitWindow(width * cellSize, height * cellSize, "Game of Life OpenMP");
    SetTargetFPS(15);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        game.drawGeneration();
        EndDrawing();

        game.computeNextGeneration();
    }

    CloseWindow();
    return 0;
}
