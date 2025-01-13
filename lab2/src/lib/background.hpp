#pragma once
#include <string>

int spawn_proc(const std::string path, int *status_code);

int wait_proc(const int pid, int *status_code);