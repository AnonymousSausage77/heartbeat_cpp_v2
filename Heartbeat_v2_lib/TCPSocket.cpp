#include "TCPSocket.hpp"

#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <chrono>

#ifdef __WIN32__
#include <Winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#endif

#include <stdio.h>
#include <unistd.h>
#include <string>
#include <string.h>

roboseals::TCPSocket::~TCPSocket()
{
    attemptClose(); 
}

roboseals::TCPSocket::TCPSocket(const int port, const std::string & ipAddress)
{
    _context->ipAddress = ipAddress;
    _context->port = port;
    this->_socketDriver = std::make_shared<std::thread>(&TCPSocket::runDriver, this);
}

void roboseals::TCPSocket::attemptClose() {
    // XXX: what happens for an already closed connection? or a connection not opened?
    // closing the connected socket
    auto monitor = _context.manuallyLock();
    
    if(isConnected()) {
        close(monitor->client_fd);
    }
}

// also reconnect (thread safe)
bool roboseals::TCPSocket::attemptConnect()
{

    auto monitor = this->_context.manuallyLock();
    // first connection
    monitor->sock = 0;
    monitor->client_fd = 0;
    monitor->writeBuffer = {};
    
    this->_isConnected = openSocket() && openConnection();
    return _isConnected;

}

void roboseals::TCPSocket::sendBytes(const char* bytes, const size_t bsize)
{
    // TODO: handle closed connection by making this add bytes to a queue and then having a 
    // new thread run over that queue to send the bytes to the server, checking for timeout as that happens
    // send a message
    auto monitor = this->_context.manuallyLock();

    std::cout << "sendBytes(" << bsize << ")" << std::endl;
    
    auto &writeBuffer = monitor->writeBuffer;
    auto currentWriteBufferSize = writeBuffer.size();
    writeBuffer.resize(currentWriteBufferSize + bsize);
    
    memcpy(writeBuffer.data() + currentWriteBufferSize, 
            bytes, 
            bsize);
            
    std::cout << "sendBytes(" << writeBuffer.size() << ")" << std::endl;

}

void roboseals::TCPSocket::sendBytes(const std::string& msg)
{
    // send a message
    sendBytes(msg.c_str(), msg.size());
}

// not thread safe
bool roboseals::TCPSocket::openSocket() 
{    
    std::cout << "Opening Socket..." << std::endl;
    
    #ifdef __WIN32__
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(2, 2);
    int winSetupResult = WSAStartup(wVersionRequested, &wsaData);
    if(winSetupResult != 0) {
        // erorr in config
        std::cout << "Error in winsock initalisation: " << winSetupResult << std::endl;
    }
    #endif
    
    if ((_context.getThreadUnsafeAccess().sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        #ifdef __WIN32__
        std::cout << "\n Socket creation error: " << WSAGetLastError() << std::endl;
        #else
        std::cout << "\n Socket creation error!" << std::endl;
        #endif
        return false;
    }
    return true;
}

// not thread safe
bool roboseals::TCPSocket::openConnection()
{
    
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(_context.getThreadUnsafeAccess().port);
 
    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    // establishing connection...
    if (inet_pton(AF_INET, _context.getThreadUnsafeAccess().ipAddress.c_str(), &serv_addr.sin_addr)
        <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return false;
    }
 
    std::cout << "Attempting connection..." << std::endl;
    if ((_context.getThreadUnsafeAccess().client_fd
         = connect(_context.getThreadUnsafeAccess().sock, (struct sockaddr*)&serv_addr,
                   sizeof(serv_addr)))
        < 0) {
        printf("\nConnection Failed \n");
        return false;
    }
    
    return true;
}

// must be accessed from driver thread
void roboseals::TCPSocket::readBytes()
{
    
    // lock with monitor
    auto monitor = _context.manuallyLock();
    
    fd_set rfd;
    FD_ZERO(&rfd);
    FD_SET(monitor->sock, &rfd);
    
    size_t totalBytesRead = 0; // can be > 1024 bytes; the sum of all buffers read
    
    // reads message
    const auto bufferSize = 1024;
    char buffer[bufferSize] = { 0 };
    // delay interval
    timeval t;
    t.tv_sec = 0;
    t.tv_usec = 60;
    
    auto socketStatus = select(monitor->sock, &rfd, nullptr, nullptr, &t);
    
    if(socketStatus < 0) {
        std::cout << "SOCKET STATUS ERROR!" << std::endl;
        this->_isConnected = false;
        return;
    } else if (socketStatus == 0) {
        std::cout << "SOCKET READ TIMED OUT" << std::endl;
        return;
    }
  //  std::cout << "SOCKET STATUS: " << socketStatus << std::endl;
    

    #ifdef __WIN32__
    int numberRead = recv(monitor->sock, buffer, bufferSize, 0);
    #else
    int numberRead = read(monitor->sock, buffer, bufferSize);
    #endif
    std::cout << "NUM BYTES: " << numberRead << std::endl;
    
    if(numberRead < 0) {
        // likely a disconnection
        std::cout << "ERROR READING BYTES" << std::endl;
        this->_isConnected = false;
        // TODO: close socket and attempt reconnect
        
    } else if(numberRead > 0) {
        auto &readBuffer = monitor->readBuffer;
        auto originalSize = readBuffer.size();
        readBuffer.resize(numberRead + readBuffer.size());
        auto endPointer =  readBuffer.data() + originalSize;
        memcpy(endPointer, buffer, numberRead); // appends the buffer contents straight into the _readBuffer vector
        totalBytesRead += numberRead;
        std::string s {buffer, numberRead}; // TESTING
        std::cout << s << std::endl; // TESTING
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


void roboseals::TCPSocket::writeBytes()
{
    auto monitor = this->context().manuallyLock();
    const size_t maxBufferSize = 1024;
    
    //std::cout << std::endl << std::endl << "TOWRTING " << monitor->writeBuffer.size() << " BYTES (" << monitor->writeBuffer.size() << ")"  << std::endl;
    //std::cout << "TEST" << std::endl;
  
  
    // XXX: should send 1024 as max buffer size
    while (monitor->writeBuffer.size() > 0) {
        size_t bsize = monitor->writeBuffer.size();
        
        std::cout << "\n\nWRITING " << bsize << " BYTES (" << monitor->writeBuffer.size() << ")"  << std::endl;
        
        std::vector<char> bytes{monitor->writeBuffer.begin(), monitor->writeBuffer.begin() + bsize};
        
        if (bsize > bytes.size()) {
            throw std::invalid_argument("Not enough bytes in vector to send!");
        }
        // send the message through the socket
        send(monitor->sock, bytes.data(), bsize, 0);
        
        // move the un-sent bytes to the start of the vector
        memcpy(monitor->writeBuffer.data(), monitor->writeBuffer.data() + bsize, bsize);
        // erase a number of bytes that have been sent from the end
        //bytes.erase(bytes.begin() + bsize, bytes.end());
        // resize the array
        monitor->writeBuffer.resize(monitor->writeBuffer.size() - bsize); 
    }
}

void roboseals::TCPSocket::runDriver()
{
    using namespace std::chrono_literals;
    while (true) {
        std::cout << "Sending: " << this->_context->writeBuffer.size() << std::endl;
        std::cout << "Reading: " << this->_context->readBuffer.size() << std::endl;
        if (this->isConnected()) {
            readBytes();
            writeBytes();
        } else {
            this->attemptConnect();
        }
        std::this_thread::sleep_for(30ms);
    }
}
