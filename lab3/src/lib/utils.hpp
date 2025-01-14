#pragma once
#include <atomic>

int cur_pid();

void proc_inc(const std::atomic_bool *is_running);

void wait_all_progs(const std::atomic_bool *is_running);