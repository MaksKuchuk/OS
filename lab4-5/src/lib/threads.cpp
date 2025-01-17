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
#include "../lib_server/db.hpp"

#ifdef __linux__
    std::string PORT = "/dev/pts/4";
#elif _WIN32
    std::string PORT = "COM4";
#endif

std::mutex SEC_MUT;
std::mutex HOUR_MUT;
std::mutex DAY_MUT;

void th_wr_sec(const std::atomic_bool *is_running, Db *db) {
    std::string p(PORT);
    cplib::SerialPort sp(p, cplib::SerialPort::BAUDRATE_115200);
    if (!std::filesystem::exists(LOG_SEC)) std::ofstream(LOG_SEC).close();

    std::string temperature;
    sp.SetTimeout(1.0);
    double temp = 0;
    while (*is_running) {
        sp >> temperature;

        try {
            temp = std::stod(temperature);
        } catch (const std::invalid_argument &e) {
            continue;
        }
        if (std::abs(temp) > 100) continue;

        time_t loc_time = get_local_time();
        SEC_MUT.lock();
        remove_out_of_period_and_add(LOG_SEC, loc_time, temp, Period::Day);
        SEC_MUT.unlock();

        db->add_to_db(loc_time, temp, Table::TSec);
    }
}

void th_wr_hour(const std::atomic_bool *is_running, Db *db) {
    while (*is_running) {
        for (size_t i = 0; i < get_period_val(Period::Hour); i++) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            if (!*is_running) break;
        }
        if (!*is_running) break;
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
        db->add_to_db(loc_time, temper, Table::THour);

        // std::vector<std::pair<time_t, double>> data;
        // db->get_from_db(&data, Table::THour);
    }
}

void th_wr_day(const std::atomic_bool *is_running, Db *db) {
    while (*is_running) {
        for (size_t i = 0; i < get_period_val(Period::Day); i++) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            if (!*is_running) break;
        }
        if (!*is_running) break;
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

        db->add_to_db(loc_time, temper, Table::TDay);
    }
}