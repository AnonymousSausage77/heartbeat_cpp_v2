#include <iostream>
#include "RXTimeFormat.hpp"
#include "RXController.hpp"
#include "RXTCPSocket.hpp"
#include "RXMessageFactory.hpp"
#include "RXHeartbeatController.hpp"
#include <optional>
#include <string>
#include "RandomStateGenerator.hpp"
#include "RX_TCP.hpp"

#define TEAM_ID "USAU"
#define PORT 2022 //?
#define ADDRESS "robotx/server" // ??
 
int main()
{
    
    
    std::cout << "REMEMBER: Set your timezone to correct timezone!" << std::endl;
    std::cout << roboseals::RX_Message::currentDateTime() << std::endl;
    
    std::cout << "attempting to resolve hostname..." << std::endl;
    
    std::optional<std::string> ipAddress = roboseals::resolveHost(ADDRESS);
    
    if(!ipAddress) {
        return -1;
    }
    
    
    std::shared_ptr<roboseals::AbstractTCPSocket> socket = std::make_shared<roboseals::RX_Message::RXTCPSocket>(PORT, *ipAddress);
    
    roboseals::RX_Message::RXSystemState state;
    roboseals::RX_Message::test::RandomStateGenerator testGen{state};
    
    //roboseals::RX_Message::RXController controller{socket, rcAdaptor}; // rcAdaptor hasnt been implemented yet
    roboseals::RX_Message::RXHeartbeatController heartbeatController{state, socket};
    heartbeatController.start();

    
    
    while(true) {} // TODO: simply for testing purposes
    
    // the thread will become trapped in the below function until the system is stopped
    //controller.start(); // starts the loop for reading and writing packets
    
    return 0;
}