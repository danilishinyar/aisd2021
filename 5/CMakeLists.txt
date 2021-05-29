cmake_minimum_required(VERSION 3.19)
project(lab5 C)

set(CMAKE_C_STANDARD 99)

add_executable(lab5 main.c lab.h graph_func.c iface.c queue.c)