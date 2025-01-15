#include<iostream>
#include<format>
#include<thread>
#include "lib/definitions.hpp"
#include "lib/my_shmem.hpp"
#include "lib/time.hpp"
#include "lib/utils.hpp"
#include "lib/ipc.hpp"

int main() {
    auto SH_MEM = cplib::SharedMem<State>(MEM_NAME);
    std::cout << "Proc1 process started" << std::endl;
    std::string str = std::string("Time: ") + time_to_string(cur_time()) + ",  Pid: " + std::to_string(cur_pid()) + ",  Process proc1";
    file_wr(std::string(LOG), &str, SH_MEM);

    SH_MEM.Lock();
    SH_MEM.Data()->counter += 10;
    SH_MEM.Unlock();

    str = std::string("Time: ") + time_to_string(cur_time()) + ",  Pid: " + std::to_string(cur_pid()) + ",  Stop proc1";
    file_wr(std::string(LOG), &str, SH_MEM);

    return 0;
}