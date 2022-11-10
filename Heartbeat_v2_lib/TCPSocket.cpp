#include "TCPSocket.hpp"

#include <iostream>

#ifdef __WIN32__
#include <Winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#endif

#include <stdio.h>
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
    auto monitor = _context.manuallyLock();
    
    if(isConnected()) {
        close(monitor->client_fd);
    }
    
    
}

// not thread safe
bool roboseals::TCPSocket::attemptConnect()
{
    bool connected = openSocket() && openConnection();
    
    return connected;
}

void roboseals::TCPSocket::sendBytes(const char* bytes, const size_t bsize)
{
    auto monitor = _context.manuallyLock();
    
    // TODO: handle closed connection
    // send a message
    send(monitor->sock, bytes, bsize, 0);
    
}

void roboseals::TCPSocket::sendBytes(const std::string& msg)
{
    auto monitor = _context.manuallyLock();
    
    // TODO: handle closed connection
    // send a message
    sendBytes(msg.c_str(), msg.size());
}

bool roboseals::TCPSocket::openSocket() 
{    
    std::cout << "Opening Socket..." << std::endl;
    
    if ((_context->sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return false;
    }
    return true;
}

bool roboseals::TCPSocket::openConnection()
{
    auto monitor = _context.manuallyLock();
    
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(monitor->port);
 
    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    // establishing connection...
    if (inet_pton(AF_INET, monitor->ipAddress.c_str(), &serv_addr.sin_addr)
        <= 0) {
        printf(
            "\nInvalid address/ Address not supported \n");
        return false;
    }
 
    std::cout << "Attempting connection..." << std::endl;
    if ((monitor->client_fd
         = connect(monitor->sock, (struct sockaddr*)&serv_addr,
                   sizeof(serv_addr)))
        < 0) {
        printf("\nConnection Failed \n");
        return false;
    }
    
    return true;
}

void roboseals::TCPSocket::readBytes()
{
    
    size_t totalBytesRead = 0; // can be > 1024 bytes
    
    // reads message
    const auto bufferSize = 1024;
    char buffer[bufferSize] = { 0 };
    int numberRead; // number of bytes read in the current frame
    
    // lock with monitor
    auto monitor = _context.manuallyLock();
    
    while (numberRead = read(monitor->sock, buffer, bufferSize) > 0) {
     
        // copy the bytes read into the _readBuffer...
        if(numberRead > 0) {
            monitor->readBuffer.resize(numberRead + monitor->readBuffer.size());
            auto endPointer =  monitor->readBuffer.data() + monitor->readBuffer.size();
            memcpy(endPointer, buffer, numberRead); // appends the buffer contents straight into the _readBuffer vector
            totalBytesRead += numberRead;
        }
        
    }
    
    
    // if there is an error
    if(numberRead < 0) {
        updateListeners(ERROR_SIGNAL, "Error reading socket bytes!");
    }
    
    // update listeners that bytes have been read
    if (totalBytesRead > 0) {
        updateListeners(DATA_RECEIVED_SIGNAL, std::to_string(totalBytesRead) + " Bytes Received...");
    }
 
}

// not thread safe
void roboseals::TCPSocket::addListener(std::weak_ptr<Observer> &listener) 
{
    this->_listeners.push_back(listener);
}


// not thread safe 
void roboseals::TCPSocket::updateListeners(int32_t signal, const std::string &message) const
{
    roboseals::update(_listeners, signal, message);
    
}

/*
template <typename T>
static void copyAccross(const T *from, size_t fromSize, std::vector<T> &dest) {
    dest.resize(fromSize); // resize the internal vector to the correct size
    memcpy(dest.data, from, fromSize); // copy the data across
   
}*/

 roboseals::TCPSocket::TCPSocket(const int port, const std::string & ipAddress)
{
    _context->ipAddress = ipAddress;
    _context->port = port;
}