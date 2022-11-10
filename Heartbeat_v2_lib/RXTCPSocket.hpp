#ifndef RXTCPSOCKET_H
#define RXTCPSOCKET_H

#include "TCPSocket.hpp"

namespace roboseals::RX_Message {
    
/**
 * @class RXTCPSocket
 * @author John Bures
 * @date 11/07/22
 * @file RXTCPSocket.hpp
 * @brief RobotX Implementation of a TCP socket for talking to the Technical Director's network.
 */
class RXTCPSocket : public TCPSocket 
{
public:
    RXTCPSocket() = delete;
    RXTCPSocket(const int port, const std::string &ipAddress) : TCPSocket(port, ipAddress) {};
    /**
     * @brief Attempts to retrieve a single packet retreived from the Technical Director's network.
     * @return A vector filled with a string of bytes constituting a message from the TD's network. If no substantial message is found, this returns
     *  and empty vector.
     */
    const std::vector<char> popBytes() override;
};
    
}

#endif // RXTCPSOCKET_H