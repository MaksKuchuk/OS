#pragma once 
#include <mutex>
#include<atomic>

void th_wr_sec(const std::atomic_bool *is_running);
void th_wr_hour(const std::atomic_bool *is_running);
void th_wr_day(const std::atomic_bool *is_running);