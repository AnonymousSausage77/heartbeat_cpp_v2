#ifndef TCPSOCKET_HPP
#define TCPSOCKET_HPP

#include <vector>
#include "AbstractTCPSocket.hpp"
#include "Monitor.hpp"
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <memory>

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


    struct TCPState {
        std::string ipAddress;
        int port;
        
        int client_fd = 0; // connection hook
        int sock = 0; // socket hook
        std::vector<char> readBuffer = {};
        std::vector<char> writeBuffer = {};
    };

    TCPSocket() = delete;
    TCPSocket(const int port, const std::string &ipAddress);
    ~TCPSocket();
    bool attemptConnect() override;
    void attemptClose() override;
    void sendBytes(const char *bytes, const size_t bsize) override;
    void sendBytes(const std::string &msg) override;
    void addListener(std::weak_ptr<Observer> &listener) override; // adds a listener for when there is data received
    bool isConnected() const override {return _isConnected; }; // might need changing to an api call rather than a stored bool
protected:
    void updateListeners(int32_t signal, const std::string &message) const  override;
    roboseals::Monitor<TCPState> &context() { return this->_context; }
    void writeBytes(); 
    void readBytes() override;
private:
    bool openSocket();
    bool openConnection();
    void runDriver();
    
    roboseals::Monitor<TCPState> _context{};
    std::vector<std::weak_ptr<Observer>> _listeners;
    std::atomic_bool _isConnected{false};

    std::mutex _socketLock;
    std::condition_variable cv; // for controlling thread waits
    std::shared_ptr<std::thread> _socketDriver;
};

}

#endif // TCPSOCKET_HPP
