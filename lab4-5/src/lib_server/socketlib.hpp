#pragma once
#include <string>

#define READ_WAIT_MS 50
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

class SocketBase {
 protected:
    SOCKET req_sock;

 public:
    SocketBase();

    int error_code();

    bool is_valid();

    void close_socket();

    void close_socket(SOCKET sock);

    int pool(const SOCKET &sock, int timeout_ms = READ_WAIT_MS);

    ~SocketBase();
};

class Request {
    std::string req_url;
    std::string req_meth;

 public:
    Request(const std::string &d);

    std::string get_meth();

    std::string get_url();
};