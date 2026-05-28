#include <iostream>
#include <chrono>
#include "GameOfLifeOMP.h"

int main() {
    int width = 1000;
    int height = 1000;

    GameOfLifeOMP game(width, height, 1);
    game.spawnGlider();

    auto start = std::chrono::high_resolution_clock::now();

    for(int i = 0; i < 2000; i++){
        game.computeNextGeneration();
    }

    auto end = std::chrono::high_resolution_clock::now();
    double time = std::chrono::duration<double, std::milli>(end - start).count();

    std::cout << "OpenMP time: " << time << " ms\n";

    return 0;
}