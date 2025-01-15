#include "background.hpp"
#include<iostream>

int spawn_proc(int argc, char** argv, int *status_code) {
    #ifdef __linux__
        pid_t pid;
        char *const argvv[] = {NULL};
        *status_code = posix_spawnp(&pid, argv[0], NULL, NULL, argvv, NULL);
        return pid;
    #elif _WIN32
        std::strgin argv_str();
        for (int i = 0; i < argc; ++i)
            argv_str += std::string(argv[i]) + ' ';
        
        PROCESS_INFORMATION process_info;
        STARTUPINFO startup_info{};
        CreateProcess(NULL, static_cast<char*>(argv_str.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &startup_info, &process_info);
        *status_code = GetLastError();
        return process_info.dwProcessId;
    #endif
}

int wait_proc(const int pid, int *status_code) {
    #ifdef __linux__
        int res;
        waitpid(pid, &res, 0);
        *status_code = WEXITSTATUS(res);
        return WTERMSIG(res);
    #elif _WIN32
        HANDLE handler = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
        int status = WaitForSingleObject(handler, INFINITE);
        GetExitCodeProcess(handler, (unsigned long*)status_code);
        CloseHandle(handler);
        return status;
    #endif
}