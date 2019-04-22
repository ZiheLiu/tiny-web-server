cmake_minimum_required(VERSION 3.10)
project(tiny_web_server)

set(CMAKE_CXX_STANDARD 11)

add_executable(tiny_web_server csapp.h csapp.c  main.c)