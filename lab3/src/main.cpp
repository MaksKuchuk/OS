#include<iostream>
#include<thread>

#include "lib/definitions.hpp"
#include "lib/background.hpp"
#include "lib/my_shmem.hpp"
#include "lib/time.hpp"
#include "lib/utils.hpp"
#include "lib/ipc.hpp"
#include "lib/threads.hpp"

int main(int argc, char** argv) {
    auto SH_MEM = cplib::SharedMem<State>("m");
    std::cout << "Main process started" << std::endl;

    std::atomic_bool is_runnig = true;
    std::string str = std::format("Time: {},  Pid: {},  Process: main", time_to_string(cur_time()), cur_pid());
    std::cout << str << std::endl;
    file_wr(std::string(LOG), &str, SH_MEM);
    SH_MEM.Lock();
    SH_MEM.Data()->processes++;
    std::thread thread_inc(th_inc, &is_runnig);
    std::thread thread_wr_log(th_wr_log, &is_runnig);
    std::thread thread_spawn_copy(th_spawn_copy, &is_runnig);
    SH_MEM.Unlock();

    int cnt = 0;
    for (;;) {
        std::string inp;
        std::cin >> inp;
        if (inp == "a") {
            std::cout << "Input num: \n";
            std::cin >> cnt;
            SH_MEM.Lock();
            SH_MEM.Data()->counter = cnt;
            SH_MEM.Unlock();
        } else if (inp == "s") {
            SH_MEM.Lock();
            std::cout << "Counter value: " << SH_MEM.Data()->counter << "\n";
            SH_MEM.Unlock();
        } else if (inp == "q") break;
    }

    SH_MEM.Lock();
    SH_MEM.Data()->processes--;
    SH_MEM.Unlock();
    is_runnig = false;
    thread_inc.join();
    thread_wr_log.join();
    thread_spawn_copy.join();

    str = std::format("Time: {},  Pid: {},  Stop main", time_to_string(cur_time()), cur_pid());
    file_wr(std::string(LOG), &str, SH_MEM);

    return 0;
}
