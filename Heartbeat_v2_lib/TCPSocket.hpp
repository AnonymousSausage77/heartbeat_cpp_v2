#ifndef TCPSOCKET_HPP
#define TCPSOCKET_HPP


#include "AbstractTCPClient.hpp"
namespace roboseals
{

class TCPSocket : virtual public AbstractTCPClient
{
public:
    TCPSocket(const int port, const std::string &ipAddress) : _port(port), _ipAddress(ipAddress) {};
    ~TCPSocket();
    bool attemptConnect() override;
    void attemptClose() override;
    void sendBytes(const uint8_t *bytes, const size_t bsize) override;
    void sendBytes(const std::string &msg) override;
private:
    bool openSocket();
    bool openConnection();
    
    int _port;
    std::string _ipAddress;
    
    int _client_fd = 0; // connection hook
    int _sock = 0; // socket hook
    bool _connected = false;

};

}

#endif // TCPSOCKET_HPP
