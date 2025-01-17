#include "socketlib.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <filesystem>
#include <fstream>

#ifdef __linux__
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
#elif WIN32
    #include <winsock2.h> /* socket */
    #include <ws2tcpip.h> /* getaddrinfo */
#endif

SocketBase::SocketBase() : req_sock(INVALID_SOCKET) {
    #ifdef __linux__
        signal(SIGPIPE, SIG_IGN);  
    #elif WIN32
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);
    #endif
}

int SocketBase::error_code() {
    #ifdef __linux__
        return errno;
    #elif WIN32
        return WSAGetLastError();  
    #endif
}

bool SocketBase::is_valid() {
    return req_sock != INVALID_SOCKET;
}

void SocketBase::close_socket() {
    close_socket(req_sock);
    req_sock = INVALID_SOCKET;
}

void SocketBase::close_socket(SOCKET sock) {
    if (sock == INVALID_SOCKET)
        return;
    #ifdef __linux__
        shutdown(sock, SHUT_WR);
        close(sock);
    #elif WIN32
        shutdown(sock, SD_SEND);
        closesocket(sock);
    #endif
}

int SocketBase::pool(const SOCKET &sock, int timeout_ms) {
    struct pollfd polstr;
    memset(&polstr, 0, sizeof(polstr));
    polstr.fd = sock;
    polstr.events |= POLLIN;

    #ifdef __linux__
        return poll(&polstr, 1, timeout_ms);
    #elif WIN32
        return WSAPoll(&polstr, 1, timeout_ms);
    #endif
}

SocketBase::~SocketBase() {
    close_socket();
    #ifdef WIN32
        WSACleanup();
    #endif
}

Request::Request(const std::string &d) {
    std::istringstream recive(d);
    std::string str;

    std::getline(recive, str);

    std::istringstream l(str);
    std::getline(l, req_meth, ' ');
    
    std::istringstream url_s(str);
    std::string url;
    std::getline(l, url, ' ');

    std::istringstream ur(url);
    std::getline(ur, req_url, '?');
    std::getline(ur, req_url, '?');
}

std::string Request::get_meth() {
    return req_meth;
}

std::string Request::get_url() {
    return req_url;
}