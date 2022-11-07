
#include "RXHeartbeatController.hpp"
#include <thread>
#include <chrono>
#include <iostream>

#define HEARTBEAT_FREQ 1000ms

roboseals::RX_Message::RXHeartbeatController::RXHeartbeatController(const RXSystemState &state, 
            std::shared_ptr<AbstractTCPSocket> socket, std::shared_ptr<RXMessageFactory> messageFactory)
            : _state(state), _socket(socket), _messageFactory(messageFactory)
{
}
void roboseals::RX_Message::RXHeartbeatController::start() 
{
    this->_isRunning = true;
    this->_clock = std::make_unique<std::thread>(&RXHeartbeatController::run, this);
    
    
}
void roboseals::RX_Message::RXHeartbeatController::stop() 
{
    this->_isRunning = false;
}


void roboseals::RX_Message::RXHeartbeatController::run() 
{
    std::cout << "starting heartbeat thread..." << std::endl;
    while (_isRunning) {
        //this->sendHeartbeat(); // not thread safe
        std::cout << "heartbeat test..." << std::endl;
        
        // wait a second
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(HEARTBEAT_FREQ);
    }
}

void roboseals::RX_Message::RXHeartbeatController::sendHeartbeat()
{
    // XXX: might not be thread safe as internal builder is shared resource
    auto message = this->_messageFactory->heartbeatMessage(_state.latitude, _state.longitude, 
        static_cast<int>(_state.systemMode), 
        static_cast<int>(_state.uavStatus));
    this->_socket->sendBytes(message);
}