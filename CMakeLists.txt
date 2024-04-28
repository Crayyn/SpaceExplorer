cmake_minimum_required(VERSION 3.27)
project(SpaceExplorer C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(SpaceExplorer
        space_explorer.c
        space_explorer.h
        space_solution.c
)
