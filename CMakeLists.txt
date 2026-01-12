cmake_minimum_required(VERSION 3.23)

project(Chip8)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS off)

# find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
add_subdirectory(vendor)
add_subdirectory(src)

target_include_directories(${PROJECT_NAME} PRIVATE include)