cmake_minimum_required(VERSION 3.10)

set(ORIGIN_SERVER origin-server)
project(${ORIGIN_SERVER})

set(CMAKE_CXX_STANDARD 11)

add_executable(${ORIGIN_SERVER} tiny.h tiny.c ../csapp.h ../csapp.c)

TARGET_LINK_LIBRARIES(${ORIGIN_SERVER} -lpthread)
