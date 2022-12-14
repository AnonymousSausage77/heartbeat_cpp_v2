#include <RXController.hpp>
#include <string>
#include <iostream>
#include <stdint.h>

using namespace roboseals::RooCOMMS;

roboseals::RX_Message::RXController::RXController(std::shared_ptr<AbstractTCPSocket> &socket, 
        std::shared_ptr<AbstractRCClientAdaptor> &rcAdaptor) 
            : _TCPsocket(socket), _rcAdaptor(rcAdaptor)
{
    this->_tcpObserver = std::make_shared<Observer>([](int32_t signal, const std::string &message)
        {
            // TODO: Add listener to TCP socket to handle acknowledgements
            std::cout << "\n\n CONTROLLER READ IN ("<< signal <<"): " << message << std::endl;
        });
        
}

void roboseals::RX_Message::RXController::start() 
{
    this->_isRunning = true;
    while (_isRunning) {
        // talk to CANBUS
        _rcAdaptor->updateState(this->_state);
        // TODO: send state heartbeat in different thread
        auto messages = _rcAdaptor->fetchMessagesToBeSent(_messageFactory);
        
        // talk to TCP server
        for (auto m : messages) {
            // TODO: queue sent bytes into acknowledgements queue
            _TCPsocket->sendBytes(m);
        }
        
        auto receivedMsg = _TCPsocket->popBytes();
        if (receivedMsg.size() > 0) {
            // XXX: DOESNT TERMINATE:
            std::string msg{receivedMsg.data(), receivedMsg.size()};
            std::cout << "\n\tmsg received: " << msg << std::endl;
        }
        
        //if(this->state().killed) {
            // close system
        //    stop();
        //}
    }
}


void roboseals::RX_Message::RXController::stop() 
{
    _isRunning = false;
}