#ifndef RXHEARTBEATCONTROLLER_H
#define RXHEARTBEATCONTROLLER_H

#include <memory>
#include <thread>
#include "AbstractTCPSocket.hpp"
#include "AbstractRCClientAdaptor.hpp"
#include "RXState.hpp"
#include "RXMessageFactory.hpp"
#include "Observer.hpp"

namespace roboseals::RX_Message {
/**
 * @class RXHeartbeatController
 * @author 
 * @date 11/07/22
 * @file RXHeartbeatController.hpp
 * @brief Runs a thread and iteratively constructs heartbeat messages and then sends them over the socket.
 *  A _state (RXSystemState) reference is kept here for the building of the heartbeat messages. However, the 
 *  fields of that _state variable are set elsewhere (ie. in RXController).
 */
class RXHeartbeatController {
public:
    RXHeartbeatController() = delete;
    RXHeartbeatController(const RXSystemState &state, 
            std::shared_ptr<AbstractTCPSocket> socket, std::shared_ptr<RXMessageFactory> messageFactory);
    void start(); // starts the thread
    void stop(); // stops the thread
private:
    void run(); // a function to be run by the thread
    void sendHeartbeat(); // constructs and sends a beartbeat over the socket
    
    bool _isRunning = false;
    std::shared_ptr<AbstractTCPSocket> _socket;
    std::shared_ptr<RXMessageFactory> _messageFactory;
    const RXSystemState &_state;
    std::unique_ptr<std::thread> _clock; 
};
    
}

#endif // RXHEARTBEATCONTROLLER_H
