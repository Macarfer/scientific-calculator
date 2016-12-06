cmake_minimum_required(VERSION 3.6)
project(scientific_calculator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.c)
add_compile_options(-lfl)
add_executable(scientific_calculator ${SOURCE_FILES})