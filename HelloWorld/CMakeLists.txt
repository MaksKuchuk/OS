cmake_minimum_required(VERSION 3.13)
project(OSProject VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
set(SRC_LIST src/main.cpp)

add_executable(${PROJECT_NAME} ${SRC_LIST})
