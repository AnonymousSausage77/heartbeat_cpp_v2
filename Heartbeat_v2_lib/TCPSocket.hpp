#ifndef TCPSOCKET_HPP
#define TCPSOCKET_HPP

#include <vector>
#include <condition_variable>
#include <mutex>
#include "AbstractTCPSocket.hpp"

namespace roboseals
{

class TCPSocket : virtual public AbstractTCPSocket
{
public:
    TCPSocket(const int port, const std::string &ipAddress) : _port(port), _ipAddress(ipAddress) {};
    ~TCPSocket();
    bool attemptConnect() override;
    void attemptClose() override;
    void sendBytes(const char *bytes, const size_t bsize) override;
    void sendBytes(const std::string &msg) override;
    void readBytes() override;
    
protected:
    std::vector<char> &readBuffer() { return _readBuffer; }
private:
    bool openSocket();
    bool openConnection();
    
    int _port;
    std::string _ipAddress;
    
    int _client_fd = 0; // connection hook
    int _sock = 0; // socket hook
    bool _connected = false;
    std::vector<char> _readBuffer;

    std::mutex _socketLock;
    std::condition_variable cv; // for controlling thread waits

};

}

#endif // TCPSOCKET_HPP
