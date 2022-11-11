#include <iostream>
#include "RXTimeFormat.hpp"
#include "RXController.hpp"
#include "RXTCPSocket.hpp"
#include "RXMessageFactory.hpp"
#include "RXHeartbeatController.hpp"
#include "RCClientAdaptor.hpp"

#define TEAM_ID "teamid"
#define PORT 2022
#define ADDRESS "127.0.0.1"
 
int main()
{
    
    std::cout << "REMEMBER: Set your timezone to correct timezone!" << std::endl;
    std::cout << roboseals::RX_Message::currentDateTime() << std::endl;
    
    roboseals::RX_Message::RXSystemState state;
    // TODO: automatically reconnect to socket if connection is lost
    std::shared_ptr<roboseals::AbstractTCPSocket> socket = std::make_shared<roboseals::RX_Message::RXTCPSocket>(PORT, ADDRESS);
    //TODO: implement rcAdaptor
    std::shared_ptr<roboseals::RooCOMMS::AbstractRCClientAdaptor> rcAdaptor = std::make_shared<roboseals::RooCOMMS::test::RCClientAdaptor>();
    
    
    if(socket->attemptConnect()) {
        std::cout << "success!" << std::endl;
    }
    
    roboseals::RX_Message::RXController controller{socket, rcAdaptor}; // rcAdaptor hasnt been implemented yet
    roboseals::RX_Message::RXHeartbeatController heartbeatController{state, socket};
    heartbeatController.start();
    
    
    //while(true) {} // TODO: simply for testing purposes
    
    // the thread will become trapped in the below function until the system is stopped
    controller.start(); // starts the loop for reading and writing packets
    
    return 0;
}