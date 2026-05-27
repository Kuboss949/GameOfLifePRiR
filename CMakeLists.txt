cmake_minimum_required(VERSION 3.18)
project(GameOfLifeCUDA LANGUAGES CXX CUDA)

set(CMAKE_CUDA_STANDARD 14)

# raylib
set(RAYLIB_PATH "D:/libs/raylib")
include_directories(${RAYLIB_PATH}/include)
link_directories(${RAYLIB_PATH}/lib)

add_executable(app main.cu)

set_target_properties(app PROPERTIES CUDA_SEPARABLE_COMPILATION ON)

target_link_libraries(app raylib opengl32 gdi32 winmm)
