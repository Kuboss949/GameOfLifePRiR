cmake_minimum_required(VERSION 3.14)
project(untitled8)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG        5.0
)
FetchContent_MakeAvailable(raylib)

add_executable(untitled8 main.cpp
        GameOfLife.h
        Board.h
        Board.cpp
        GameOfLife.cpp)

target_link_libraries(untitled8 PRIVATE raylib)