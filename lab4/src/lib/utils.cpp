#include "utils.hpp"
#include <filesystem>
#include <chrono>
#include <fstream>
#include <iostream>

std::string get_date_from_log(std::string str) {
    return str.substr(0, str.find(','));
}

std::string get_temp_from_log(std::string str) {
    return str.substr(str.find(':'), str.size());
}

double get_avg_temp(std::ifstream &file, time_t loc_time, Period per) {
    std::string str;
    double temp = 0;
    while (std::getline(file, str)) {
        if (str.empty()) continue;
        time_t time_string = std::stoll(get_date_from_log(str));

        if (!is_diff_more(loc_time, time_string, per)) {
            temp += std::stod(get_temp_from_log(str));
            break;
        }
    }
    if (file.eof()) return temp;
    
    while (std::getline(file, str)) {
        temp += std::stod(get_temp_from_log(str));
    }
    return temp;
}

time_t get_local_time() {
    auto now = std::chrono::system_clock::now();
    time_t t = std::chrono::system_clock::to_time_t(now);

    return t;
}

bool is_diff_more(time_t t1, time_t t2, Period per) {
    time_t diff = std::abs(t1 - t2);
    if (per == Period::Hour) {
        return diff > 60 * 60;
    } else if (per == Period::Day) {
        return diff > 60 * 60 * 24;
    } else if (per == Period::Month) {
        return diff > 60 * 60 * 24 * 30;
    } else if (per == Period::Year) {
        return diff > 60 * 60 * 24 * 30 * 365;
    }
    return false;
}

void remove_out_of_period_and_add(std::ofstream &loc, std::string path, time_t loc_time, double temp, Period per) {
    std::string tmp_path = path + ".tmp";
    if (std::filesystem::exists(tmp_path)) std::filesystem::remove(tmp_path);

    std::filesystem::copy_file(path, tmp_path);
    std::ifstream tmp(tmp_path);

    std::string str;
    while (std::getline(tmp, str)) {
        if (str.empty()) continue;
        time_t time = std::stoll(get_date_from_log(str));
        std::cout << time << std::endl;

        if (!is_diff_more(loc_time, time, per)) break;
    }
    if (!tmp.eof()) loc << str << '\n';
    
    while (std::getline(tmp, str)) loc << str << '\n';
    loc << std::format("{},  Temp: {}", loc_time, temp);

    tmp.close();
    std::filesystem::remove(tmp_path);
}