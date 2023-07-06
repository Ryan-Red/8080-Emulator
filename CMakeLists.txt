cmake_minimum_required(VERSION 3.16.0)

project(Intel8080_sim)

include_directories(include)


add_executable (decompiler src/decompiler.c)
add_executable (Intel8080 src/Intel8080.cpp)