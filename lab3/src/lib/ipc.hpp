#pragma once
#include <fstream>
#include "threads.hpp"
#include "my_shmem.hpp"

void file_wr(std::string path, std::string *str, cplib::SharedMem<State> &sh_data);

int prog_count(cplib::SharedMem<State> &sh_data);
