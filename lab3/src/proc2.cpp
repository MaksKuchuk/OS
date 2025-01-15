#include<iostream>
#include<format>
#include<thread>
#include<chrono>
#include "lib/definitions.hpp"
#include "lib/my_shmem.hpp"
#include "lib/time.hpp"
#include "lib/utils.hpp"
#include "lib/ipc.hpp"

int main() {
    auto SH_MEM = cplib::SharedMem<State>(MEM_NAME);
    // std::cout << "Proc2 process started" << std::endl;
    std::string str = std::string("Time: ") + time_to_string(cur_time()) + ",  Pid: " + std::to_string(cur_pid()) + ",  Process: proc2";
    file_wr(std::string(LOG), &str, SH_MEM);

    SH_MEM.Lock();
    SH_MEM.Data()->counter *= 2;
    SH_MEM.Data()->copies++;
    SH_MEM.Unlock();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    SH_MEM.Lock();
    SH_MEM.Data()->counter /= 2;
    SH_MEM.Data()->copies--;
    SH_MEM.Unlock();

    str = std::string("Time: ") + time_to_string(cur_time()) + ",  Pid: " + std::to_string(cur_pid()) + ",  Stop proc2";
    file_wr(std::string(LOG), &str, SH_MEM);

    return 0;
}