#include <iostream>
#include <string>
#include <chrono>
#include <format>
#include <thread>
#include <fstream>
#include <atomic>

#include "lib/threads.hpp"
#include "lib/definitions.hpp"

int main(int argc, char **argv) {
    std::atomic_bool is_runnig = {true};

    std::thread th_sec(th_wr_sec, &is_runnig);
    std::thread th_hour(th_wr_hour, &is_runnig);
    std::thread th_day(th_wr_day, &is_runnig);

    std::string str;
    while (is_runnig) {
        std::cout << "[q]uit: ";
        std::cin >> str;
        if (str == "q") {
            is_runnig = false;
        }
    }

    th_sec.join();
    th_hour.join();
    th_day.join();

    return 0;
}