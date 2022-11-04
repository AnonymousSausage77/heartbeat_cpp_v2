#ifndef TCPCLIENT_HPP
#define TCPCLIENT_HPP

#include <stdint.h>
#include "Observer.hpp"

namespace roboseals
{

// data received observer
typedef long (*data_received_func)(const uint8_t*, const uint32_t);

class AbstractTCPClient 
{
    public:
    AbstractTCPClient();
    ~AbstractTCPClient();
    virtual void sendBytes(const uint8_t *bytes, const size_t bsize) = 0; // sends data to the server
    virtual void sendBytes(const std::string &msg) = 0;
    virtual void addDataReceivedListener(Observer listener) = 0; // adds a listener for when there is data received
    virtual bool isConnected() const = 0; // checks if the socket is connected
    virtual bool attemptConnect() = 0;
    virtual void attemptClose() = 0;

    protected:

    virtual void updateDataReceivedListeners() const = 0;


};

}

#endif // TCPCLIENT_HPP
