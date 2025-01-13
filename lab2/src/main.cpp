#include<iostream>
#include "lib/background.hpp"

int main(int argc, char* argv[]) {
    std::string path = std::string(argv[1]);

    std::cout << path << std::endl;

    int spawn_status;
    int pid = spawn_proc(path, &spawn_status);

    int exit_code;
    wait_proc(pid, &exit_code);

    std::cout << std::endl << "Name: " << argv[0] << ",  pid: " << pid << ",  exit_code: " << exit_code << std::endl;

    return 0;
}
