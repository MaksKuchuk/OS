#include "background.hpp"

#ifdef __linux__
    #include <wait.h>
    #include <spawn.h>
#elif _WIN32
    #include <windows.h>
#endif

int spawn_proc(const std::string path, int &status_code) {
    #ifdef __linux__
        pid_t pid;
        char *const argv[] = {NULL};
        status_code = posix_spawnp(&pid, path.c_str(), NULL, NULL, argv, NULL);
        return pid;
    #elif _WIN32
        PROCESS_INFORMATION process_info;
        STARTUPINFO startup_info{};
        int success = CreateProcess(path, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
        status_code = GetLastError();
        return pi.dwProcessId;
    #endif
}

int wait_proc(const int pid, int &status_code) {
    #ifdef __linux__
        int res;
        waitpid(pid, &res, 0);
        status_code = WEXITSTATUS(res);
        return WTERMSIG(res);
    #elif _WIN32
        HANDLE handler = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
        int status = WaitForSingleObject(handler, INFINITE);
        GetExitCodeProcess(handle, status_code);
        CloseHandle(handle);
        return status;
    #endif
}