cmake_minimum_required(VERSION 3.10)

set(PROJECT_11_6 11.6)
project(${PROJECT_11_6})

set(CMAKE_CXX_STANDARD 11)

add_executable(${PROJECT_11_6} tiny.h tiny.c ../csapp.h ../csapp.c)

TARGET_LINK_LIBRARIES(${PROJECT_11_6} -lpthread)
