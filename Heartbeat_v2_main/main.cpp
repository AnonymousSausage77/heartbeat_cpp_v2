#include <iostream>
#include "RXTimeFormat.hpp"
#include "RXController.hpp"
#include "RXTCPSocket.hpp"
#include "RXMessageFactory.hpp"

#define TEAM_ID "teamid"
#define PORT 2000
#define ADDRESS "192.168.1.2"
 
int main()
{
    std::cout << "REMEMBER: Set your timezone to correct timezone!" << std::endl;
    std::cout << roboseals::RX_Message::currentDateTime() << std::endl;
    
    auto factory = std::make_shared<roboseals::RX_Message::RXMessageFactory>(TEAM_ID);
    auto socket = std::make_shared<roboseals::RX_Message::RXTCPSocket>(PORT, ADDRESS);
    //TODO: auto rcAdaptor = std::make_shared<?>;
    
    socket->attemptConnect();
    
    // NOTE: the lifetime of this object depends on its internal while loop
    //roboseals::RX_Message::RXController controller;
    
    //controller.start();
    
    return 0;
}