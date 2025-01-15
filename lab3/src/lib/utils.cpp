#include "utils.hpp"
#include "definitions.hpp"

int cur_pid() {
    #ifdef __linux__
        return getpid();
    #elif _WIN32
        return GetCurrentProcessId();
    #endif
}

void wait_all_progs(const std::atomic_bool *is_running) {
    auto SH_MEM = cplib::SharedMem<State>(MEM_NAME);
    int loc_pid = cur_pid();
    do {
        SH_MEM.Lock();
        int closed_pid = SH_MEM.Data()->closed_pid;
        if (closed_pid == 0) SH_MEM.Data()->closed_pid = loc_pid;
        SH_MEM.Unlock();

        if (closed_pid == loc_pid) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    } while (*is_running);
}