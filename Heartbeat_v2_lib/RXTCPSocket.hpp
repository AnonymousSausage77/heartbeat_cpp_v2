#ifndef RXTCPSOCKET_H
#define RXTCPSOCKET_H

#include "TCPSocket.hpp"

namespace roboseals::RX_Message {
    
class RXTCPSocket : public TCPSocket 
{
public:
    RXTCPSocket() = delete;
    RXTCPSocket(const int port, const std::string &ipAddress) : TCPSocket(port, ipAddress) {};
    const std::vector<char> popBytes() override;
    
    
};
    
    
}


#endif // RXTCPSOCKET_H