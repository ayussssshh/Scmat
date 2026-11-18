cmake_minimum_required(VERSION 3.20)
project(mve C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)   # this is what clangd reads

find_package(SDL3 REQUIRED CONFIG)



add_subdirectory(src/graphic)
add_subdirectory(src/app)