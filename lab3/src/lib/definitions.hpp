#pragma once
#include "threads.hpp"
#include "my_shmem.hpp"

#ifdef __linux__
    #define PROC1 "./build/proc1"
    #define PROC2 "./build/proc2"
#elif _WIN32
    #define PROC1 "./build/proc1.exe"
    #define PROC2 "./build/proc2.exe"
#endif

#define MEM_NAME "123123"
#define LOG "./log.txt"