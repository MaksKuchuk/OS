#include "utils.hpp"
#include <sstream>
#include <fstream>
#include <iostream>
#include "db.hpp"
#include "server.hpp"
#include "../lib/definitions.hpp"

void th_server(const std::atomic_bool *is_running, Db *db) {
    HttpServer serv(IP, IP_PORT, db);
    if (!serv.is_valid()) {
        std::cerr << "Server error" << std::endl;
        return;
    }

    while (*is_running) {
        if (!serv.is_valid()) {
            std::cerr << "Server error" << std::endl;
            break;
        }
        serv.process_client();
    }
}