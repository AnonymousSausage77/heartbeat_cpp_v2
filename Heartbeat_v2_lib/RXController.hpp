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
    
/**
 * @class RXController
 * @author John Bures
 * @date 11/07/22
 * @file RXController.hpp
 * @brief Directs the movement of packets from the RooCOMMS CANBUS network to the Technical Director's network. This class
 *  maintains an internal model of the current boat (stored in '_state'), updating it as changes come in from the CANBUS.
 * TODO: this class is largly incomplete because we changed last minute to only do the basic, laptop-based heartbeat
 */
class RXController {
public:
    RXController(std::shared_ptr<AbstractTCPSocket> &socket, std::shared_ptr<AbstractRCClientAdaptor> &rcAdaptor);
    void start();
    void stop();
    const RXSystemState &state() const;
private:
    RXSystemState _state;
    std::shared_ptr<AbstractTCPSocket> _TCPsocket; // thread safe
    std::shared_ptr<AbstractRCClientAdaptor> _rcAdaptor; 
    std::atomic_bool _isRunning{false};
    RXMessageFactory _messageFactory;
    
    std::shared_ptr<Observer> _tcpObserver;
};
    
}

#endif // RXCONTROLLER_H
