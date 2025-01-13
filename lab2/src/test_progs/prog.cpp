#include<iostream>
#ifdef __linux__
    #include <unistd.h>
#elif _WIN32
    #include <windows.h>
#endif

void slp(int m) {
    #ifdef __linux__
        usleep(m * 1000); 
    #elif _WIN32
        Sleep(m);
    #endif
} 

int main(int argc, char* argv[]) {
    std::string str = std::string("prog in running: [                         ]   0%\r");
    std::string s = std::string();
    slp(300);

    for (int i = 1; i <= 100; ++i) {
        fflush(stdout);

        if (i % 4 == 0)
            str[17 + i / 4] = '#';

        s = std::to_string(i);
        for (size_t j = 0; j < s.size(); ++j) {
            str[47 - j] = s[s.size() - 1 - j];
        }

        std::cout << str;
        slp(25);
    }
    std::cout << std::endl;

    return 0;
}