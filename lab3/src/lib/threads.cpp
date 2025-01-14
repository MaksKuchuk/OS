#include "threads.hpp"
#include "definitions.hpp"
#include "utils.hpp"
#include "ipc.hpp"
#include "background.hpp"
#include "time.hpp"

void th_inc(const std::atomic_bool *is_running) {
    auto SH_MEM = cplib::SharedMem<State>("m");
    while (*is_running && SH_MEM.IsValid()) {
        SH_MEM.Lock();
        SH_MEM.Data()->counter++;
        SH_MEM.Unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
}

void th_spawn_copy(const std::atomic_bool *is_running) {
    wait_all_progs(is_running);
    auto SH_MEM = cplib::SharedMem<State>("m");

    while (SH_MEM.IsValid() && *is_running) {
        SH_MEM.Lock();
        if (SH_MEM.Data()->copies == 0) {
            int status;
            char *argv[] = {PROC1};
            spawn_proc(1, argv, &status);

            argv[0] = PROC2;
            spawn_proc(1, argv, &status);
        } else {
            std::string str = std::format("Time: {},  Pid: {},  Can't start new copies", time_to_string(cur_time()), cur_pid());
            file_wr(std::string(LOG), &str, SH_MEM);
        }
        SH_MEM.Unlock();
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}

void th_wr_log(const std::atomic_bool *is_running) {
    wait_all_progs(is_running);
    auto SH_MEM = cplib::SharedMem<State>("m");

    while (SH_MEM.IsValid() && *is_running) {
        SH_MEM.Lock();
        std::string str = std::format("Time: {},  Pid: {},  Counter: {}", time_to_string(cur_time()), cur_pid(), SH_MEM.Data()->counter);
        SH_MEM.Unlock();
        file_wr(std::string(LOG), &str, SH_MEM);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    SH_MEM.Lock();
    SH_MEM.Data()->closed_pid = (SH_MEM.Data()->closed_pid == cur_pid()) ? -1 : SH_MEM.Data()->closed_pid;
    SH_MEM.Unlock();
}

