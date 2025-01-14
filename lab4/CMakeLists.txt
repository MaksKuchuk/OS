cmake_minimum_required(VERSION 3.13)
project(OSProject VERSION 1.0 LANGUAGES CXX)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set(LIB_HDRS src/lib/background.hpp)
set(LIB_SRCS src/lib/background.cpp)

add_library(background ${LIB_HDRS} ${LIB_SRCS})

add_executable(prog src/test_progs/prog.cpp)
add_executable(error_prog src/test_progs/error_prog.cpp)
add_executable(main src/main.cpp ${LIB_HDRS} ${LIB_SRCS})
