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
    std::unique_lock<std::mutex> lock{this->_socketLock};
    cv.wait(lock);
    
    if(_connected) {
        close(_client_fd);
        _connected = false;
    }
    
    
    cv.notify_one();
}

// not thread safe
bool roboseals::TCPSocket::attemptConnect()
{
    _connected = openSocket() && openConnection();
    
    return _connected;
}

void roboseals::TCPSocket::sendBytes(const char* bytes, const size_t bsize)
{
    std::unique_lock<std::mutex> lock{this->_socketLock};
    cv.wait(lock);
    
    // TODO: handle closed connection
    // send a message
    send(_sock, bytes, bsize, 0);
    
    cv.notify_one();
}

void roboseals::TCPSocket::sendBytes(const std::string& msg)
{
    // TODO: handle closed connection
    // send a message
    sendBytes(msg.c_str(), msg.size());
}

// not thread safe
bool roboseals::TCPSocket::openSocket() 
{    
    std::cout << "Opening Socket..." << std::endl;
    
    if ((_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return false;
    }
    return true;
}

// not thread safe
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

void roboseals::TCPSocket::readBytes()
{
    
    size_t totalBytesRead = 0; // can be > 1024 bytes
    
    // reads message
    const auto bufferSize = 1024;
    char buffer[bufferSize] = { 0 };
    int numberRead; // number of bytes read in the current frame
    
    // lock socket mutex
    std::unique_lock<std::mutex> lock{this->_socketLock};
    cv.wait(lock);
    
    while (numberRead = read(_sock, buffer, bufferSize) > 0) {
     
        // copy the bytes read into the _readBuffer...
        if(numberRead > 0) {
            _readBuffer.resize(numberRead + _readBuffer.size());
            auto endPointer =  _readBuffer.data() + _readBuffer.size();
            memcpy(endPointer, buffer, numberRead); // appends the buffer contents straight into the _readBuffer vector
            totalBytesRead += numberRead;
        }
        
    }
    
    cv.notify_one();
    
    // if there is an error
    if(numberRead < 0) {
        updateListeners(ERROR_SIGNAL, "Error reading socket bytes!");
    }
    
    // update listeners that bytes have been read
    if (totalBytesRead > 0) {
        updateListeners(DATA_RECEIVED_SIGNAL, std::to_string(totalBytesRead) + " Bytes Received...");
    }
 
}

void roboseals::TCPSocket::addListener(std::weak_ptr<Observer> &listener) 
{
    this->_listeners.push_back(listener);
}


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