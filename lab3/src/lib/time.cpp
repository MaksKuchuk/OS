#include "time.hpp"

loc_time_struct cur_time() {
    auto now = std::chrono::system_clock::now();
    time_t time = std::chrono::system_clock::to_time_t(now);
    tm *loc_time = localtime(&time);
    loc_time_struct loc_time_obj = loc_time_struct{loc_time, (unsigned long long)now.time_since_epoch().count() % 1000};
    return loc_time_obj;
}

std::string time_to_string(loc_time_struct time) {
    return std::format("{:0>4}-{:0>2}-{:0>2} {:0>2}:{:0>2}:{:0>2}.{:0>3}",
                        time.loc_time->tm_year + 1900,
                        time.loc_time->tm_mon + 1,
                        time.loc_time->tm_mday,
                        time.loc_time->tm_hour,
                        time.loc_time->tm_min,
                        time.loc_time->tm_sec,
                        time.milliseconds);
}