cmake_minimum_required(VERSION 3.13)
project(OSProject VERSION 1.0 LANGUAGES CXX)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

# set(CMAKE_CXX_COMPILER "clang")

# if (UNIX)
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23")
# endif (UNIX)
# if (WIN32)
#     # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23")
    set(CMAKE_CXX_STANDARD 23)
# endif (WIN32)

set(LIB_HDRS src/lib/threads.hpp src/lib/utils.hpp src/lib/my_serial.hpp src/lib/definitions.hpp)
set(LIB_SRCS src/lib/threads.cpp src/lib/utils.cpp)

add_executable(main src/main.cpp ${LIB_HDRS} ${LIB_SRCS})
add_executable(sim src/sim.cpp src/lib/my_serial.hpp src/lib/utils.hpp src/lib/utils.cpp)

# IF (NOT WIN32)
#     TARGET_LINK_LIBRARIES(main rt pthread)
#     TARGET_LINK_LIBRARIES(proc1 rt pthread)
#     TARGET_LINK_LIBRARIES(proc2 rt pthread)
# ENDIF (NOT WIN32)