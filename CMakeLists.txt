cmake_minimum_required(VERSION 3.15)

project(
    Minesweeper
    VERSION 1.0
    DESCRIPTION "A simple terminal minesweeper application with arrow controls"
    LANGUAGES C)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_C_EXTENSIONS OFF)
    set(CMAKE_C_STANDARD 11)
    set(CMAKE_C_STANDARD_REQUIRED ON)

    add_subdirectory(src)
endif()