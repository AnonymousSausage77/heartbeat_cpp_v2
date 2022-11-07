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
    
class RXHeartbeatController {
public:
    RXHeartbeatController() = delete;
    RXHeartbeatController(const RXSystemState &state, 
            std::shared_ptr<AbstractTCPSocket> socket, std::shared_ptr<RXMessageFactory> messageFactory);
    void start();
    void stop();
private:
    void run(); // runs by thread
    void sendHeartbeat();
    bool _isRunning = false;
    std::shared_ptr<AbstractTCPSocket> _socket;
    std::shared_ptr<RXMessageFactory> _messageFactory;
    const RXSystemState &_state;
    std::unique_ptr<std::thread> _clock; 
};
    
}

#endif // RXHEARTBEATCONTROLLER_H
