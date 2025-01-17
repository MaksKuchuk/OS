#include "server.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <filesystem>
#include <fstream>

#ifdef WIN32

#include <winsock2.h> /* socket */
#include <ws2tcpip.h> /* getaddrinfo */

#else

#include <sys/socket.h> /* socket */
#include <sys/types.h>
#include <netinet/in.h> /* socket */
#include <arpa/inet.h>  /* socket */
#include <unistd.h>
#include <netdb.h> /* getaddrinfo */
#include <poll.h>  /* poll */
#include <signal.h>
#include <string.h> /* memset */
#define SOCKET int
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

#endif

#include "socketlib.hpp"
#include "../lib/definitions.hpp"

static std::string get_ans(const std::string &b) {
    std::stringstream ans;
    ans << "HTTP/1.1 200 OK\r\n" << "Content-Type: text/plain\r\n" << "Content-Length: " << b.length() << "\r\n\r\n" << b;
    return ans.str();
}

std::string get_data_from_db(Db *db,  Table table) {
    std::vector<std::pair<time_t, double>> data;
    db->get_from_db(&data, table);
    std::string str("");
    if (!data.empty()) {
        for (auto item : data)
            str += std::to_string(item.first) + ' ' + std::to_string(item.second) + ' ';
        str.pop_back();
    }
    return str;
}

HttpServer::HttpServer(const std::string &ip, const short int port, Db* db) {
    listen_(ip, port);
    this->db = db;
}

void HttpServer::listen_(const std::string &ip, const short int port) {
    if (req_sock != INVALID_SOCKET)  close_socket();
    struct addrinfo h;
    memset(&h, 0, sizeof(h));
    h.ai_flags = AI_PASSIVE;
    h.ai_protocol = IPPROTO_TCP;
    h.ai_socktype = SOCK_STREAM;
    h.ai_family = AF_INET;

    struct addrinfo *ad = NULL;
    int res = getaddrinfo(ip.c_str(), std::to_string(port).c_str(), &h, &ad);

    if (res != 0) {
        std::cerr << "Failed getaddrinfo: " << res << std::endl;
        freeaddrinfo(ad);
        return;
    }

    req_sock = socket(ad->ai_family, ad->ai_socktype, ad->ai_protocol);
    if (req_sock == INVALID_SOCKET) {
        std::cerr << "Cant open socket: " << error_code() << std::endl;
        freeaddrinfo(ad);
    }

    if (bind(req_sock, ad->ai_addr, ad->ai_addrlen) == SOCKET_ERROR) {
        std::cerr << "Failed to bind: " << error_code() << std::endl;
        freeaddrinfo(ad);
        close_socket();
    }
    freeaddrinfo(ad);

    if (listen(req_sock, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Failed to start listen: " << error_code() << std::endl;
        close_socket();
    }
}

void HttpServer::process_client() {
    if (!is_valid()) {
        std::cerr << "Server (listening) socket is invalid!" << std::endl;
        return;
    }

    SOCKET client_socket = accept(req_sock, NULL, NULL);
    if (client_socket == INVALID_SOCKET) {
        std::cerr << "Error accepting client: " << error_code() << std::endl;
        close_socket(client_socket);
        return;
    }

    if (pool(client_socket) <= 0) {
        close_socket(client_socket);
        return;
    }

    std::stringstream recv_str;
    int buf_size = sizeof(req_buf) - 1;
    int result = -1;
    do {
        result = recv(client_socket, req_buf, buf_size, 0);
        if (result < 0) break;
        req_buf[result] = '\0';
        recv_str << req_buf;
    } while (result >= buf_size);

    if (result == SOCKET_ERROR || result < 0) {
        std::cerr << "Error on client receive: " << error_code() << std::endl;
        close_socket(client_socket);
        return;
    } else if (result == 0) {
        std::cerr << "Client closed connection before getting any data!" << std::endl;
        close_socket(client_socket);
        return;
    }

    auto req = Request(recv_str.str());
    // std::cout << recv_str.str() << "\n\n";

    // std::cout << req.get_meth() << " " << req.get_url() << std::endl;
    
    std::string d;
    if (req.get_url() == "/sec") {
        d = get_data_from_db(db, Table::TSec);
    } else if (req.get_url() == "/hour") {
        d = get_data_from_db(db, Table::THour); }
    else if (req.get_url() == "/day") {
        d = get_data_from_db(db, Table::TDay);
    }

    auto response = get_ans(d);

    result = send(client_socket, response.c_str(), (int)response.length(), 0);
    if (result == SOCKET_ERROR) {
        std::cerr << "Failed to send response to client: " << error_code() << std::endl;
    }
    
    close_socket(client_socket);
    // std::cout << "Answered to client!" << std::endl;
}

