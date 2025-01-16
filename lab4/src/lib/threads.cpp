#include "threads.hpp"
#include <mutex>
#include <chrono>
#include <thread>
#include <fstream>
#include <iostream>
#include <filesystem>
#include "definitions.hpp"
#include "utils.hpp"
#include "my_serial.hpp"

#ifdef __linux__
    std::string PORT = "/dev/pts/4";
#elif _WIN32
    std::string PORT = "COM4";
#endif

std::mutex SEC_MUT;
std::mutex HOUR_MUT;
std::mutex DAY_MUT;

void th_wr_sec() {
    std::string p(PORT);
    cplib::SerialPort sp(p, cplib::SerialPort::BAUDRATE_115200);
    if (!std::filesystem::exists(LOG_SEC)) std::ofstream(LOG_SEC).close();

    std::string temperature;
    sp.SetTimeout(1.0);
    double temp = 0;
    while (true) {
        sp >> temperature;

        try {
            temp = std::stod(temperature);
        } catch (const std::invalid_argument &e) {
            continue;
        }

        time_t loc_time = get_local_time();
        SEC_MUT.lock();
        remove_out_of_period_and_add(LOG_SEC, loc_time, temp, Period::Day);
        SEC_MUT.unlock();
    }
}

void th_wr_hour() {
    for (;;) {
        std::this_thread::sleep_for(std::chrono::seconds(get_period_val(Period::Hour)));
        if (!std::filesystem::exists(LOG_HOUR)) std::ofstream(LOG_HOUR).close();

        SEC_MUT.lock();
        time_t loc_time = get_local_time();
        std::ifstream sec_log(LOG_SEC);
        double temper = get_avg_temp(sec_log, loc_time, Period::Hour);
        sec_log.close();
        SEC_MUT.unlock();

        HOUR_MUT.lock();
        remove_out_of_period_and_add(LOG_HOUR, loc_time, temper, Period::Month);
        HOUR_MUT.unlock();
    }
}

void th_wr_day() {
    for (;;) {
        std::this_thread::sleep_for(std::chrono::seconds(get_period_val(Period::Day)));
        if (!std::filesystem::exists(LOG_DAY)) std::ofstream(LOG_DAY).close();

        HOUR_MUT.lock();
        time_t loc_time = get_local_time();
        std::ifstream hour_log(LOG_HOUR);
        double temper = get_avg_temp(hour_log, loc_time, Period::Day);
        hour_log.close();
        HOUR_MUT.unlock();

        DAY_MUT.lock();
        remove_out_of_period_and_add(LOG_DAY, loc_time, temper, Period::Year);
        DAY_MUT.unlock();
    }
}