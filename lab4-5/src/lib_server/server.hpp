#pragma once
#include <string>
#include "socketlib.hpp"
#include "db.hpp"

class HttpServer : public SocketBase {
    char req_buf[1024];
    Db* db;

 public:
    HttpServer(const std::string &ip, const short int port, Db* db);

    void listen_(const std::string &ip, const short int port);

    void process_client();
};
