#pragma once
#include "threads.hpp"
#include "my_shmem.hpp"

#ifdef __linux__
    #define PROC1 "./proc1"
    #define PROC2 "./proc2"
#elif _WIN32
    #define PROC1 "./proc1.exe"
    #define PROC2 "./proc2.exe"
#endif

// #define SH_MEM cplib::SharedMem<State>("mem")

// shlib::SharedMem<State> SH_MEM() {
//     return shlib::SharedMem<State>("m");
// }

#define LOG "./log.txt"