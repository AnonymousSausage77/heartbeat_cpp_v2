#include "RX_TCP.hpp"

#include <cstring>
#ifdef __WIN32__
#include <Winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#endif
#include <iostream>

std::optional<std::string> roboseals::resolveHost(const std::string &ipHostAddress) {
    WSADATA wsdata;
    addrinfo hints, *res;
    WSAStartup (MAKEWORD (2, 2), &wsdata);

    memset(&hints, 0, sizeof hints);

    hints.ai_family     = AF_UNSPEC;
    hints.ai_socktype   = SOCK_STREAM;
    hints.ai_protocol   = IPPROTO_TCP;

    // checks if the DNS host can be resolved
    if (getaddrinfo(ipHostAddress.c_str(), NULL, &hints, &res) != 0){
        std::cout << "could not resolve DNS host " << ipHostAddress << std::endl;
        return {}; // cannot be resolved
        
    }

    // gets the ip address
    return inet_ntoa(((sockaddr_in *) res -> ai_addr) -> sin_addr);
}