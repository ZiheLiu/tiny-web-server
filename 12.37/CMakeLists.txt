cmake_minimum_required(VERSION 3.10)

set(PROJECT_12_37 12.37)
project(${PROJECT_12_37})

set(CMAKE_CXX_STANDARD 11)

add_executable(${PROJECT_12_37} tiny.h tiny.c ../csapp.h ../csapp.c)

TARGET_LINK_LIBRARIES(${PROJECT_12_37} -lpthread)
