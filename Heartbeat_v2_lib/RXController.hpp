#ifndef RXCONTROLLER_H
#define RXCONTROLLER_H

#include <memory>
#include "AbstractTCPSocket.hpp"
#include "AbstractRCClientAdaptor.hpp"
#include "RXState.hpp"
#include "RXMessageFactory.hpp"
#include "Observer.hpp"

using namespace roboseals::RooCOMMS;

namespace roboseals::RX_Message{
    
class RXController {
public:
    RXController(std::shared_ptr<RXMessageFactory> &factory, std::shared_ptr<AbstractTCPSocket> &socket, std::shared_ptr<AbstractRCClientAdaptor> &rcAdaptor);
    void start();
    void stop();
    
private:
    RXSystemState _state;
    std::shared_ptr<AbstractTCPSocket> _TCPsocket;
    std::shared_ptr<AbstractRCClientAdaptor> _rcAdaptor;
    bool _isRunning = false;
    std::shared_ptr<RXMessageFactory> _messageFactory;
    std::shared_ptr<Observer> _tcpObserver;
};
    
}

#endif // RXCONTROLLER_H
