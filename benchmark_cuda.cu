#include <iostream>
#include <vector>
#include <iomanip>
#include <cuda_runtime.h>

// Kernel (deklaracja)
__global__ void gameOfLifeKernel(uint8_t* current, uint8_t* next, int width, int height);

// pojedynczy benchmark
float runBenchmark(int size, int generations, dim3 blockSize)
{
    int width = size;
    int height = size;

    size_t totalSize = width * height * sizeof(uint8_t);

    // host
    std::vector<uint8_t> hostBoard(width * height, 0);

    // prosta inicjalizacja (glider)
    int cx = width / 2;
    int cy = height / 2;
    hostBoard[(cy - 1) * width + cx] = 1;
    hostBoard[cy * width + (cx + 1)] = 1;
    hostBoard[(cy + 1) * width + (cx - 1)] = 1;
    hostBoard[(cy + 1) * width + cx] = 1;
    hostBoard[(cy + 1) * width + (cx + 1)] = 1;

    // device
    uint8_t *d_current, *d_next;
    cudaMalloc(&d_current, totalSize);
    cudaMalloc(&d_next, totalSize);

    cudaMemcpy(d_current, hostBoard.data(), totalSize, cudaMemcpyHostToDevice);

    dim3 gridSize(
        (width + blockSize.x - 1) / blockSize.x,
        (height + blockSize.y - 1) / blockSize.y
    );

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    cudaEventRecord(start);

    for (int i = 0; i < generations; i++)
    {
        gameOfLifeKernel<<<gridSize, blockSize>>>(d_current, d_next, width, height);
        std::swap(d_current, d_next);
    }

    cudaEventRecord(stop);
    cudaEventSynchronize(stop);

    float ms = 0;
    cudaEventElapsedTime(&ms, start, stop);

    cudaFree(d_current);
    cudaFree(d_next);
    cudaEventDestroy(start);
    cudaEventDestroy(stop);

    return ms / 1000.0f; // sekundy
}

int main()
{
    std::vector<int> sizes = {200, 500, 1000};
    std::vector<int> generations = {100, 500, 1000};

    // sensowny default do testów
    dim3 blockSize(16, 16);

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "===== GAME OF LIFE CUDA BENCHMARK =====\n\n";
    std::cout << "Block size: " << blockSize.x << "x" << blockSize.y << "\n\n";

    for (int size : sizes)
    {
        std::cout << "Plansza: " << size << " x " << size << "\n";

        for (int gen : generations)
        {
            float time = runBenchmark(size, gen, blockSize);

            std::cout << "  Generacje: " << std::setw(4) << gen
                      << " -> czas: " << time << " s\n";
        }

        std::cout << "\n";
    }

    return 0;
}