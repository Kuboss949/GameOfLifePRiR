cmake_minimum_required(VERSION 3.14)
project(GameOfLifeOMP)

set(CMAKE_CXX_STANDARD 17)

# OpenMP
find_package(OpenMP REQUIRED)

# ===== RAYLIB =====
include(FetchContent)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.0
)

FetchContent_MakeAvailable(raylib)

# ===== EXECUTABLE =====
add_executable(app main.cpp GameOfLifeOMP.cpp)

target_link_libraries(app PRIVATE
    OpenMP::OpenMP_CXX
    raylib
    opengl32
    gdi32
    winmm
)