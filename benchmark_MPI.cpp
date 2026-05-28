#include <mpi.h>
#include <iostream>
#include <vector>
#include "GameOfLifeMPI.h"

// Funkcja benchmarkuj¹ca
double runBenchmark(GameOfLifeMPI& game, int generations) {
    MPI_Barrier(MPI_COMM_WORLD);

    double start = MPI_Wtime();

    for (int i = 0; i < generations; i++) {
        game.computeNextGeneration();
    }

    MPI_Barrier(MPI_COMM_WORLD);

    double end = MPI_Wtime();
    double local = end - start;

    double global;
    MPI_Reduce(&local, &global, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    return global;
}

void runBenchmarkMode() {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<int> grids = { 200, 500, 1000 };
    std::vector<int> gens = { 100, 500, 1000 };

    for (int g : grids) {
        for (int gen : gens) {

            GameOfLifeMPI game(g, g, 1, rank, size);
            game.spawnGlider();

            double time = runBenchmark(game, gen);

            if (rank == 0) {
                std::cout << "Grid: " << g
                    << "x" << g
                    << ", Generations: " << gen
                    << ", Processes: " << size
                    << ", Time: " << time << " s" << std::endl;
            }
        }
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    runBenchmarkMode();

    MPI_Finalize();
    return 0;
}
