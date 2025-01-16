#pragma once
#include <string>
#include <ctime>

enum Period {
    Hour,
    Day,
    Month,
    Year
};

double get_avg_temp(std::ifstream &file, time_t loc_time, Period per);

time_t get_local_time();

std::string get_date_from_log(std::string str);

bool is_diff_more(time_t t1, time_t t2, Period per);

void remove_out_of_period_and_add(std::ofstream &loc, std::string path, time_t loc_time, double temp, Period per);