#include "utils.hpp"
#include <filesystem>
#include <chrono>
#include <fstream>
#include <iostream>

std::string get_date_from_log(std::string str) {
    size_t t = str.find(',');
    return str.substr(0, t);
}

std::string get_temp_from_log(std::string str) {
    return str.substr(str.find(':') + 2, str.size());
}

size_t get_period_val(Period per) {
    if (per == Period::Hour) {
        return 3;
    } else if (per == Period::Day) {
        return 7;
    } else if (per == Period::Month) {
        return 15;
    } else if (per == Period::Year) {
        return 31;
    }

    if (per == Period::Hour) {
        return 60 * 60;
    } else if (per == Period::Day) {
        return 60 * 60 * 24;
    } else if (per == Period::Month) {
        return 60 * 60 * 24 * 30;
    } else if (per == Period::Year) {
        return 60 * 60 * 24 * 30 * 365;
    }

    return 0;
}

bool is_diff_more(time_t t1, time_t t2, Period per) {
    time_t diff = std::abs(t1 - t2);
    return diff > get_period_val(per);
}

double get_avg_temp(std::ifstream &file, time_t loc_time, Period per) {
    std::string str;
    double temp = 0;
    size_t cnt = 1;
    while (std::getline(file, str)) {
        if (str.empty()) continue;
        time_t time_string = std::stoll(get_date_from_log(str));

        if (!is_diff_more(loc_time, time_string, per)) break;
    }
    std::string sss = get_temp_from_log(str);


    temp += std::stod(sss);
    // if (file.eof()) return temp;
    
    while (std::getline(file, str)) {
        cnt++;
        temp += std::stod(get_temp_from_log(str));
    }
    return temp / cnt;
}

time_t get_local_time() {
    auto now = std::chrono::system_clock::now();
    time_t t = std::chrono::system_clock::to_time_t(now);

    return t;
}

void remove_out_of_period_and_add(std::string path, time_t loc_time, double temp, Period per) {
    std::string tmp_path = path + ".tmp";
    if (std::filesystem::exists(tmp_path)) std::filesystem::remove(tmp_path);

    std::filesystem::copy_file(path, tmp_path);
    std::ifstream tmp(tmp_path);
    std::ofstream loc(path);

    std::string str;
    time_t time;
    while (std::getline(tmp, str)) {
        if (str.empty()) continue;

        try {
            time = std::stoll(get_date_from_log(str));
        } catch (const std::invalid_argument &e) {
            continue;
        }

        if (!is_diff_more(loc_time, time, per)) break;
    }
    // if (!tmp.eof()) loc << str << '\n';
    loc << str << '\n';
    
    while (std::getline(tmp, str)) {
        if (str.empty()) continue;
        loc << str << '\n';
    }

    loc << std::format("{},  Temp: {}", loc_time, temp);

    loc.close();
    tmp.close();
    std::filesystem::remove(tmp_path);
}