cmake_minimum_required(VERSION 3.10)

set(PROEJCT_12_36_2 12.36.2)
project(${PROEJCT_12_36_2})

set(CMAKE_CXX_STANDARD 11)

add_executable(${PROEJCT_12_36_2} echoservers.h echoservers.c tiny.h tiny.c ../csapp.h ../csapp.c main.c)

TARGET_LINK_LIBRARIES(${PROEJCT_12_36_2} -lpthread)
