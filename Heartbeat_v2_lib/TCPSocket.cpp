#include "TCPSocket.hpp"

#include <iostream>
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <string.h>

roboseals::TCPSocket::~TCPSocket()
{
    attemptClose(); 
}

void roboseals::TCPSocket::attemptClose() {
    // XXX: what happens for an already closed connection? or a connection not opened?
    // closing the connected socket
    close(_client_fd);
    _connected = false;
}

bool roboseals::TCPSocket::attemptConnect()
{
    _connected = openSocket() && openConnection();
     
    
    // read message
    //char buffer[1024] = { 0 };
    //int valread = read(_sock, buffer, 1024);
    //printf("%s\n", buffer);
 
    return _connected;
}

void roboseals::TCPSocket::sendBytes(const uint8_t* bytes, const size_t bsize)
{
    // TODO: handle closed connection
    // send a message
    send(_sock, bytes, bsize, 0);
}

void roboseals::TCPSocket::sendBytes(const std::string& msg)
{
    // TODO: handle closed connection
    // send a message
    send(_sock, msg.c_str(), msg.length(), 0);
}

bool roboseals::TCPSocket::openSocket()
{    
    std::cout << "Opening Socket..." << std::endl;
    
    if ((_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return false;
    }
    return true;
}

bool roboseals::TCPSocket::openConnection()
{
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(_port);
 
    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    // establishing connection...
    if (inet_pton(AF_INET, _ipAddress.c_str(), &serv_addr.sin_addr)
        <= 0) {
        printf(
            "\nInvalid address/ Address not supported \n");
        return false;
    }
 
    std::cout << "Attempting connection..." << std::endl;
    if ((_client_fd
         = connect(_sock, (struct sockaddr*)&serv_addr,
                   sizeof(serv_addr)))
        < 0) {
        printf("\nConnection Failed \n");
        return false;
    }
    
    return true;
}
