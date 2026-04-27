#include <iostream>
#include "raylib.h"
#include "GameOfLife.h"

int main() {
    InitWindow(1500, 1000, "Gra w Zycie");
    SetTargetFPS(15);

    std::unique_ptr<GameOfLife> game = std::make_unique<GameOfLife>(150, 100, 10);
    game->spawnGlider();
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(RAYWHITE);
        game->drawGeneration();
        EndDrawing();
        game->computeNextGeneration();
    }


    return 0;
}
