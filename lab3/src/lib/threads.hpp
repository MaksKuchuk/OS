#pragma once
#include<atomic>
#include<thread>

struct State {
    int closed_pid = 0;
    int copies = 0;
    int processes = 0;

    int counter = 0;
};

void th_inc(const std::atomic_bool *is_running);

void th_spawn_copy(const std::atomic_bool *is_thread_running);

void th_wr_log(const std::atomic_bool *is_thread_running);