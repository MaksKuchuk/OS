#pragma once
#include<string>
#include<atomic>

#ifdef __linux__
    #include <wait.h>
    #include <spawn.h>
#elif _WIN32
    #include <windows.h>
#endif

int spawn_proc(int argc, char** argv, int *status_code);

int wait_proc(const int pid, int *status_code);