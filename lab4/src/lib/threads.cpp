#include "threads.hpp"
#include <mutex>
#include <chrono>
#include <thread>
#include <fstream>
#include <iostream>
#include "definitions.hpp"
#include "utils.hpp"
#include "my_serial.hpp"

#ifdef __linux__
    std::string PORT = "/dev/pts/3";
#elif _WIN32
    std::string PORT = "COM3";
#endif

std::mutex SEC_MUT;
std::mutex HOUR_MUT;
std::mutex DAY_MUT;

void th_wr_sec() {
    std::string p(PORT);
    cplib::SerialPort sp(p, cplib::SerialPort::BAUDRATE_115200);

    std::string temperature;
    sp.SetTimeout(1.0);
    while (true) {
        sp >> temperature;

        time_t loc_time = get_local_time();
        SEC_MUT.lock();
        std::ofstream sec_log(LOG_SEC);
        remove_out_of_period_and_add(sec_log, LOG_SEC, loc_time, std::stod(temperature), Period::Day);
        SEC_MUT.unlock();
    }
}

void th_wr_hour() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    // std::this_thread::sleep_for(std::chrono::hours(1));
    SEC_MUT.lock();
    time_t loc_time = get_local_time();
    std::ifstream sec_log(LOG_SEC);
    double temper = get_avg_temp(sec_log, loc_time, Period::Hour);
    sec_log.close();
    SEC_MUT.unlock();

    HOUR_MUT.lock();
    std::ofstream hour_log(LOG_HOUR);
    remove_out_of_period_and_add(hour_log, LOG_HOUR, loc_time, temper, Period::Day);
    hour_log.close();
    HOUR_MUT.unlock();
}

void th_wr_day() {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    // std::this_thread::sleep_for(std::chrono::days(1));
    HOUR_MUT.lock();
    time_t loc_time = get_local_time();
    std::ifstream hour_log(LOG_HOUR);
    double temper = get_avg_temp(hour_log, loc_time, Period::Day);
    hour_log.close();
    HOUR_MUT.unlock();

    DAY_MUT.lock();
    std::ofstream day_log(LOG_DAY);
    remove_out_of_period_and_add(day_log, LOG_DAY, loc_time, temper, Period::Year);
    hour_log.close();
    DAY_MUT.unlock();
}