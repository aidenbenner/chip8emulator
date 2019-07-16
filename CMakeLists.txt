cmake_minimum_required(VERSION 3.12)
project(chip8cpp)

set(CMAKE_CXX_STANDARD 14)

add_executable(chip8cpp main.cpp cpu.cc cpu.h chip8.h Memory.h macros.h)

file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})