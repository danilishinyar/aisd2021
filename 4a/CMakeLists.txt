cmake_minimum_required(VERSION 3.19)
project(lab4a C)

set(CMAKE_C_STANDARD 99)

add_executable(lab4a main.c lab.h tree_func.c iface.c)