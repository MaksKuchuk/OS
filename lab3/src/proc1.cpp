#include<iostream>
#include<format>
#include<thread>
#include "lib/definitions.hpp"
#include "lib/my_shmem.hpp"
#include "lib/time.hpp"
#include "lib/utils.hpp"
#include "lib/ipc.hpp"

int main() {
    auto SH_MEM = cplib::SharedMem<State>("m");
    std::cout << "Proc1 process started" << std::endl;
    std::string str = std::format("Time: {},  Pid: {},  Process: proc1", time_to_string(cur_time()), cur_pid());
    file_wr(std::string(LOG), &str, SH_MEM);

    SH_MEM.Lock();
    SH_MEM.Data()->counter += 10;
    SH_MEM.Unlock();

    str = std::format("Time: {},  Pid: {},  Stop proc1", time_to_string(cur_time()), cur_pid());
    file_wr(std::string(LOG), &str, SH_MEM);

    return 0;
}