#ifndef TCPCLIENT_HPP
#define TCPCLIENT_HPP

#include <stdint.h>
#include <vector>
#include "Observer.hpp"

namespace roboseals
{

const inline int32_t ERROR_SIGNAL = -1;
const inline int32_t DATA_RECEIVED_SIGNAL = 0;
    
class AbstractTCPSocket 
{
    public:
    AbstractTCPSocket();
    ~AbstractTCPSocket();
    virtual void sendBytes(const char *bytes, const size_t bsize) = 0; // sends data to the server
    virtual void sendBytes(const std::string &msg) = 0;
    virtual void addListener(Observer &listener) = 0; // adds a listener for when there is data received
    virtual bool isConnected() const = 0; // checks if the socket is connected
    virtual bool attemptConnect() = 0;
    virtual void attemptClose() = 0;
    virtual const std::vector<char> popBytes() = 0; // removes bytes from the buffer and returns them; returns an empty array if no bytes are found
    virtual void readBytes() = 0;
    
    protected:
    virtual void updateListeners(int32_t signal, const std::string &message) const = 0;


};

}

#endif // TCPCLIENT_HPP
