cmake_minimum_required(VERSION 3.23)

project(Chip8)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS off)

add_executable(chip8
    src/main.cpp
    src/chip8.cpp
)

target_include_directories(chip8 PRIVATE include)

