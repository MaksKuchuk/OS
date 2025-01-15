#include "ipc.hpp"
#include "threads.hpp"
#include "my_shmem.hpp"
#include<iostream>

void file_wr(std::string path, std::string *str, cplib::SharedMem<State> &sh_data) {
    sh_data.Lock();
    std::ofstream log(path, std::ios::app);
    // std::cout << (*str) << std::endl;
    log << (*str).c_str() << '\n';
    log.close();
    sh_data.Unlock();
}

int prog_count(cplib::SharedMem<State> &sh_data) {
    sh_data.Lock();
    int count = sh_data.Data()->processes;
    sh_data.Unlock();
    return count;
}