cmake_minimum_required(VERSION 3.26)

project(GameOfLifeCUDA LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CUDA_STANDARD 17)

set(CMAKE_CUDA_HOST_COMPILER "D:/Visual studio/VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64/cl.exe")

find_package(raylib REQUIRED)

add_executable(GameOfLifeCUDA
    main.cpp
    GameOfLifeCUDA.cu
    GameOfLifeCUDA.h
)

set_target_properties(GameOfLifeCUDA PROPERTIES
    CUDA_SEPARABLE_COMPILATION ON
    CUDA_ARCHITECTURES 75 86 89
)

target_link_libraries(GameOfLifeCUDA PRIVATE raylib)

if(UNIX)
    target_link_libraries(GameOfLifeCUDA PRIVATE m pthread dl)
endif()