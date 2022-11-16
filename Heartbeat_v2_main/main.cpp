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
#include <thread>
#include <chrono>

#define TEAM_ID "USAU"
#define PORT 12345 // ?
#define ADDRESS "robot.server" // ??
//#define ADDRESS "google.com"
int main()
{
    
    std::cout << "PORT: " << PORT << std::endl;
    std::cout << "HOST: " << ADDRESS << std::endl;
    
    std::cout << "REMEMBER: Set your timezone to correct timezone!" << std::endl;
    std::cout << roboseals::RX_Message::currentDateTime() << std::endl;
    
    std::cout << "attempting to resolve hostname..." << std::endl;
    
    std::optional<std::string> ipAddress = roboseals::resolveHost(ADDRESS);
    
    
    if(!ipAddress) {
        std::cout << "NO RESOLVED IP: CLOSING..." << std::endl;
        ipAddress  = {"192.168.200.222"};
       // using namespace std::chrono_literals;
        //std::this_thread::sleep_for(1s);
        //return -1;
    }
    
    std::cout << "RESOLVED_IP: " << ipAddress.value() << std::endl;
    
    
    std::shared_ptr<roboseals::AbstractTCPSocket> socket = std::make_shared<roboseals::RX_Message::RXTCPSocket>(PORT, *ipAddress);
    
    roboseals::RX_Message::RXSystemState state;
    roboseals::RX_Message::test::RandomStateGenerator testGen{state};
    
    //roboseals::RX_Message::RXController controller{socket, rcAdaptor}; // rcAdaptor hasnt been implemented yet
    roboseals::RX_Message::RXHeartbeatController heartbeatController{state, socket};
    heartbeatController.start();

    
    
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(15s);
    
    //while(true) {} // TODO: simply for testing purposes
    
    // the thread will become trapped in the below function until the system is stopped
    //controller.start(); // starts the loop for reading and writing packets
    
    return 0;
}