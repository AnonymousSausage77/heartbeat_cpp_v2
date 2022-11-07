#ifndef TCPSOCKET_HPP
#define TCPSOCKET_HPP

#include <vector>
#include <condition_variable>
#include <mutex>
#include "AbstractTCPSocket.hpp"

namespace roboseals
{

/**
 * @class TCPSocket
 * @author John Bures 
 * @date 11/07/22
 * @file TCPSocket.hpp
 * @brief A virtual socket that defines most of the functionality for the TCP socket. This is not specific to the RobotX challenge, however,
 *  and is reusable in other situations. It is threadsafe (unless recorded elsewhere).
 */
class TCPSocket : virtual public AbstractTCPSocket
{
public:
    TCPSocket() = delete;
    TCPSocket(const int port, const std::string &ipAddress) : _port(port), _ipAddress(ipAddress) {};
    ~TCPSocket();
    bool attemptConnect() override;
    void attemptClose() override;
    void sendBytes(const char *bytes, const size_t bsize) override;
    void sendBytes(const std::string &msg) override;
    void readBytes() override;
    void addListener(std::weak_ptr<Observer> &listener) override; // adds a listener for when there is data received
    bool isConnected() const {return _connected;}; // might need changing to an api call rather than a stored bool
protected:
    std::vector<char> &readBuffer() { return _readBuffer; }
    void updateListeners(int32_t signal, const std::string &message) const  override;
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
    std::vector<std::weak_ptr<Observer>> _listeners;

};

}

#endif // TCPSOCKET_HPP
