cmake_minimum_required(VERSION 3.10)

set(PROJECT_12_38 12.38)
project(${PROJECT_12_38})

set(CMAKE_CXX_STANDARD 11)

add_executable(${PROJECT_12_38} tiny.h tiny.c ../csapp.h ../csapp.c sbuf.c sbuf.h)

TARGET_LINK_LIBRARIES(${PROJECT_12_38} -lpthread)
