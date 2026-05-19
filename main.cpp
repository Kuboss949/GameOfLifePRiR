
/* WERSJA SEKWENCYJNA */
/*
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
*/


/* WERSJA OPEN MPI */
#include <iostream>
#include <vector>
#include <mpi.h>
#include "raylib.h"
#include "GameOfLifeMPI.h"

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int global_width = 150;
    int global_height = 100;
    int cell_size = 10;

    std::unique_ptr<GameOfLifeMPI> game = std::make_unique<GameOfLifeMPI>(global_width, global_height, cell_size, rank, size);

    // Spawn odpalamy na wszystkich, ale lambda w środku sprawdza do kogo należy ten kawałek
    game->spawnGlider();

    if (rank == 0) {
        InitWindow(global_width * cell_size, global_height * cell_size, "Gra w Zycie - MPI");
        SetTargetFPS(15);
    }

    int keepRunning = 1;
    std::vector<uint8_t> fullBoardBuffer;

    while (true) {
        if (rank == 0) {
            keepRunning = !WindowShouldClose();
        }

        MPI_Bcast(&keepRunning, 1, MPI_INT, 0, MPI_COMM_WORLD);

        if (!keepRunning) break;

        // Każdy proces wysyła swój fragment, proces 0 zbiera go w fullBoardBuffer
        game->gatherBoardToMaster(fullBoardBuffer);

        if (rank == 0) {
            BeginDrawing();
            ClearBackground(RAYWHITE);
            game->drawGeneration(fullBoardBuffer);
            EndDrawing();
        }

        // Wszystkie procesy synchronicznie liczą kolejną generację
        game->computeNextGeneration();
    }

    if (rank == 0) {
        CloseWindow();
    }

    MPI_Finalize();
    return 0;
}