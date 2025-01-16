#include <iostream>
#include <string>
#include <chrono>
#include <format>
#include <thread>
#include <fstream>

#include "lib/threads.hpp"
#include "lib/definitions.hpp"


int main(int argc, char **argv) {
    std::thread th_sec(th_wr_sec);
    std::thread th_hour(th_wr_hour);
    std::thread th_day(th_wr_day);

    for (;;) {}

    th_sec.join();
    th_hour.join();
    th_day.join();

    return 0;
}