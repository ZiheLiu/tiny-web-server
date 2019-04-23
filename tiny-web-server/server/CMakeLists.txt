cmake_minimum_required(VERSION 3.10)

set(SERVER server)
project(${SERVER})

set(CMAKE_CXX_STANDARD 11)

add_executable(${SERVER} echoservers.h echoservers.c tiny.h tiny.c sbuf.c sbuf.h ../csapp.h ../csapp.c main.c)

TARGET_LINK_LIBRARIES(${SERVER} -lpthread)
