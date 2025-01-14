cmake_minimum_required(VERSION 3.13)
project(OSProject VERSION 1.0 LANGUAGES CXX)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

# set(CMAKE_CXX_COMPILER "clang")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23")

set(LIB_HDRS src/lib/background.hpp src/lib/ipc.hpp src/lib/threads.hpp src/lib/time.hpp src/lib/utils.hpp src/lib/my_shmem.hpp src/lib/definitions.hpp)
set(LIB_SRCS src/lib/background.cpp src/lib/ipc.cpp src/lib/threads.cpp src/lib/time.cpp src/lib/utils.cpp)

add_executable(main src/main.cpp ${LIB_HDRS} ${LIB_SRCS})
add_executable(proc1 src/proc1.cpp ${LIB_HDRS} ${LIB_SRCS})
add_executable(proc2 src/proc2.cpp ${LIB_HDRS} ${LIB_SRCS})

IF (NOT WIN32)
    TARGET_LINK_LIBRARIES(main rt pthread)
    TARGET_LINK_LIBRARIES(proc1 rt pthread)
    TARGET_LINK_LIBRARIES(proc2 rt pthread)
ENDIF (NOT WIN32)