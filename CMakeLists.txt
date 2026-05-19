cmake_minimum_required(VERSION 3.14)
project(untitled8)

set(CMAKE_CXX_STANDARD 20)

# 1. Pobranie ścieżek z systemowych zmiennych środowiskowych MS-MPI
set(MPI_CXX_INCLUDE_PATH "$ENV{MSMPI_INC}")
set(MPI_CXX_LIBRARIES "$ENV{MSMPI_LIB64}/msmpi.lib")

# Sprawdzenie, czy ścieżki istnieją (zapobiega błędom w późniejszym etapie)
if(NOT MPI_CXX_INCLUDE_PATH OR NOT EXISTS "${MPI_CXX_LIBRARIES}")
    message(FATAL_ERROR "Nie znaleziono MS-MPI. Upewnij się, że MS-MPI SDK jest zainstalowane.")
endif()

# 2. Dodanie katalogu z nagłówkami MPI
include_directories(${MPI_CXX_INCLUDE_PATH})

# 3. Pobranie i konfiguracja Raylib
include(FetchContent)

FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG        5.0
)
FetchContent_MakeAvailable(raylib)

# 4. Zdefiniowanie pliku wykonywalnego
add_executable(untitled8 main.cpp
        GameOfLife.h
        GameOfLifeMPI.h
        Board.h
        Board.cpp
        GameOfLife.cpp
        GameOfLifeMPI.cpp)

# 5. Linkowanie Raylib oraz MS-MPI do projektu
target_link_libraries(untitled8 PRIVATE raylib ${MPI_CXX_LIBRARIES})