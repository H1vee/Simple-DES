cmake_minimum_required(VERSION 3.30)
project(S_DES)

set(CMAKE_CXX_STANDARD 14)

include_directories(src/core)

add_executable(S_DES
    src/core/SDES.cpp
    src/core/SDES.h
    src/main.cpp)
