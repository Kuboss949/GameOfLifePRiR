
cudaEvent_t start, stop;
cudaEventCreate(&start);
cudaEventCreate(&stop);

// init GPU once
cudaMemcpy(d_current, hostBoard.data(), size, cudaMemcpyHostToDevice);

cudaEventRecord(start);

for(int i = 0; i < 1000; i++){
    gameOfLifeKernel<<<gridSize, blockSize>>>(d_current, d_next, width, height);
    std::swap(d_current, d_next);
}

cudaEventRecord(stop);
cudaEventSynchronize(stop);

float ms = 0;
cudaEventElapsedTime(&ms, start, stop);

printf("CUDA time: %f ms\n", ms);
