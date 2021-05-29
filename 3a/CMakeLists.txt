cmake_minimum_required(VERSION 3.19)
project(lab3 C)

set(CMAKE_C_STANDARD 99)

add_executable(lab3 lab.h main.c iface.c tb_func.c )
