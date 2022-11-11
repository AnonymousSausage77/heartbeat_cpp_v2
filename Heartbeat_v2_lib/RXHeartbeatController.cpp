
#include "RXHeartbeatController.hpp"
#include <thread>
#include <chrono>
#include <iostream>

#define HEARTBEAT_FREQ 1000ms

roboseals::RX_Message::RXHeartbeatController::RXHeartbeatController(const RXSystemState &state, 
            std::shared_ptr<AbstractTCPSocket> socket)
            : _state(state), _socket(socket)
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
        this->sendHeartbeat();
        std::cout << "heartbeat test..." << std::endl;
        
        // wait a second
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(HEARTBEAT_FREQ);
    }
}

void roboseals::RX_Message::RXHeartbeatController::sendHeartbeat()
{
    std::cout << "create heartbeat" << std::endl;
    auto message = this->_messageFactory.heartbeatMessage(_state.latitude, _state.longitude, 
        static_cast<int>(_state.systemMode.load()), 
        static_cast<int>(_state.uavStatus.load()));
        
    std::cout << "send heartbeat" << std::endl;
    this->_socket->sendBytes(message);
    
    std::cout << "sent heartbeat" << std::endl;
}