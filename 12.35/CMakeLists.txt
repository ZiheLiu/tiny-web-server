cmake_minimum_required(VERSION 3.10)

set(PROEJCT_12_35 12.35)
project(${PROEJCT_12_35})

set(CMAKE_CXX_STANDARD 11)

add_executable(${PROEJCT_12_35} tiny.h tiny.c ../csapp.h ../csapp.c)

TARGET_LINK_LIBRARIES(${PROEJCT_12_35} -lpthread)
