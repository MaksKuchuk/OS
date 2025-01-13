#ifdef __linux__
    #include <wait.h>
    #include <spawn.h>
#elif _WIN32
    #include <windows.h>
#endif

int spawn_proc(int argc, char** argv, int *status_code) {
    #ifdef __linux__
        pid_t pid;
        char *const argv[] = {NULL};
        status_code = posix_spawnp(&pid, path.c_str(), NULL, NULL, argv, NULL);
        return pid;
    #elif _WIN32
        PROCESS_INFORMATION process_info;
        STARTUPINFO startup_info{};
        CreateProcess(path.c_str(), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &startup_info, &process_info);
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