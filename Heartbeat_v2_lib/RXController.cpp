#include <RXController.hpp>
#include <string>
#include <iostream>
#include <stdint.h>


using namespace roboseals::RooCOMMS;

roboseals::RX_Message::RXController::RXController(std::shared_ptr<RXMessageFactory> &factory, std::shared_ptr<AbstractTCPSocket> &socket, 
        std::shared_ptr<AbstractRCClientAdaptor> &rcAdaptor) 
            : _messageFactory(factory), _TCPsocket(socket), _rcAdaptor(rcAdaptor)
{
    this->_tcpObserver = std::make_shared<Observer>([](int32_t signal, const std::string &message)
        {
            // TODO: Add listener to TCP socket to handle acknowledgements
        });
        
}

void roboseals::RX_Message::RXController::start() 
{
    while (_isRunning) {
        // talk to CANBUS
        _rcAdaptor->updateState(this->_state);
        // TODO: send state heartbeat in different thread
        auto messages = _rcAdaptor->fetchMessagesToBeSent(*_messageFactory);
        
        // talk to TCP server
        for (auto m : messages) {
            // TODO: queue sent bytes into acknowledgements queue
            _TCPsocket->sendBytes(m);
        }
        _TCPsocket->readBytes();
        
        if(this->state().killed) {
            // close system
            stop();
        }
    }
}


void roboseals::RX_Message::RXController::stop() 
{
    _isRunning = false;
}