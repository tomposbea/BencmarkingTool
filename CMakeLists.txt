project(BenchmarkingTool)
cmake_minimum_required(VERSION 3.12)

set (DCMAKE_C_FLAGS "-o -pthread")

add_executable(main main.c)
add_executable(lcm lcm.c)
set(HEADERS includes.h)
