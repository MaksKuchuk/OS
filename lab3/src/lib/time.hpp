#pragma once
#include<string>
#include<time.h>
#include<chrono>

struct loc_time_struct {
    tm* loc_time;
    unsigned long long milliseconds;
};

loc_time_struct cur_time();

std::string time_to_string(loc_time_struct local_time);
