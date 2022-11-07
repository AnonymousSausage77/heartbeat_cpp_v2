#ifndef RXTCPSOCKET_H
#define RXTCPSOCKET_H

#include "TCPSocket.hpp"

namespace roboseals::RX_Message {
    
class RXTCPSocket : public TCPSocket 
{
public:
    const std::vector<char> popBytes() override;
    
    
};
    
    
}


#endif // RXTCPSOCKET_H