cmake_minimum_required(VERSION 3.13)
project(OSProject VERSION 1.0 LANGUAGES CXX)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")


find_package(SQLite3 REQUIRED)

# set(CMAKE_CXX_COMPILER "clang")

# if (UNIX)
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23")
# endif (UNIX)
# if (WIN32)
#     # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23")
    set(CMAKE_CXX_STANDARD 23)
# endif (WIN32)

set(LIB_HDRS src/lib_server/db.hpp src/lib_server/server.hpp src/lib_server/socketlib.hpp src/lib_server/utils.hpp src/lib/threads.hpp src/lib/utils.hpp src/lib/my_serial.hpp src/lib/definitions.hpp src/lib_server/sql.hpp)
set(LIB_SRCS src/lib_server/db.cpp src/lib_server/server.cpp src/lib_server/socketlib.cpp src/lib_server/utils.cpp src/lib/threads.cpp src/lib/utils.cpp)

add_executable(main src/main.cpp ${LIB_HDRS} ${LIB_SRCS})
add_executable(sim src/sim.cpp src/lib/my_serial.hpp src/lib/utils.hpp src/lib/utils.cpp)

target_link_libraries(main PRIVATE SQLite::SQLite3)

# IF (NOT WIN32)
#     TARGET_LINK_LIBRARIES(main rt pthread)
#     TARGET_LINK_LIBRARIES(proc1 rt pthread)
#     TARGET_LINK_LIBRARIES(proc2 rt pthread)
# ENDIF (NOT WIN32)