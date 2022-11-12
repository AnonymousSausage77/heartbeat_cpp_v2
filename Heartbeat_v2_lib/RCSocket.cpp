#include "RCSocket.hpp"

extern "C" {
#include "can_control.h"
}

roboseals::RooCOMMS::RCSocket::RCSocket() {
    
    // construct the CANBUS connection
    std::cout << "Attempting to connect to CANBUS..." << std::endl;
    auto c = *InitialiseCAN(can0, _socket);
    std::cout << c << std::endl;
    
    if(*c) {
        std::cout << "ERROR SETTING UP CAN" << std::endl;
    }
 
}

roboseals::RooCOMMS::RCSocket::RCSocket~() {
    
	CloseCAN(can0, *_socket);
    
    // on the heap?
    delete _socket;
 
}
