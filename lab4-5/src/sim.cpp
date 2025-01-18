#include "lib/my_serial.hpp"
#include "lib/utils.hpp"
#include <iostream>
#include <thread>
#include <string>
#include <chrono>
#include <cmath>
#include <format>

#define _USE_MATH_DEFINES

#ifdef __linux__
    std::string PORT = "/dev/pts/0";
#elif _WIN32
    std::string PORT = "COM3";
#endif

double temp(double month, double day, double sec) {
    double t = month + (day + sec / (24 * 60 * 60)) / 30;
    return -std::cos(t * M_PI / 6) * 17 + 4 - std::cos(t * M_PI * 60) * 2;
}

int main(int argc, char **argv) {
    std::string p(PORT);
    cplib::SerialPort sp(p, cplib::SerialPort::BAUDRATE_115200);

    if (!sp.IsOpen()) {
        std::cout << "Can't open: " << p << std::endl;
        return -1;
    }

    std::string str;
    for (;;) {
        auto now = std::chrono::system_clock::now();
        time_t t = std::chrono::system_clock::to_time_t(now);
        tm tt = *localtime(&t);

        double month = tt.tm_mon;
        double day = tt.tm_mday;
        double seconds = tt.tm_hour * 60 * 60 + tt.tm_min * 60 + tt.tm_sec;
        str = std::to_string(temp(month, day, seconds));
        sp << str;
        std::cout << str;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    return 0;
}