#include "RXTCPSocket.hpp"
#include <string>
#include <iostream>
#include <cstring>
#include "RXMessageBuilder.hpp"

const std::vector<char> roboseals::RX_Message::RXTCPSocket::popBytes() {
    //std::cout << "interpret bytes... (" << std::to_string(monitor->readBuffer.size()) << " bytes)" << std::endl;
    bool headerFound = false;

    int ptrIndex = 0;
    // lock socket state from other threads
    auto monitor = context().manuallyLock();
    auto currentBufferSize = monitor->readBuffer.size();
    // no bytes to read
    if(currentBufferSize <= 0) {
        return {};
    }
    std::cout << "pop Buffer size: " << currentBufferSize << std::endl;
    for(; ptrIndex < currentBufferSize; ptrIndex++) {
        // search for packet start
        if((uint8_t) monitor->readBuffer.at(ptrIndex) == (uint8_t) '$') {
            std::cout << "VAL:" << monitor->readBuffer.at(ptrIndex) << std::endl; 
            headerFound = true;
            break;
        }
    }

    if(!headerFound) {
        // if there is no packet start code, the packet is not found then do not read
        return {};
    }
    
    // place of the '$'
    const auto startMarkerIndex = ptrIndex;

    // get the first '$' marker in the buffered bytes
    auto ptr = monitor->readBuffer.begin() + startMarkerIndex;

    size_t packlength = 0;
    bool foundEndMarker = false;
    for(auto itr = monitor->readBuffer.begin() + startMarkerIndex; itr < monitor->readBuffer.end(); itr++) {
        packlength++;
        if ((*itr) == '*') {
            packlength += 2; // XXX: ensure that checksum is always 2 characters (currently not sure if it is)
            foundEndMarker = true;
            break;
        }
    }

    if (!foundEndMarker) {
        // buffer does not contain '*' character
        return {};
    } else if (startMarkerIndex + packlength > monitor->readBuffer.size()) {
        // buffer does not contain msg (including checksum, etc.)
        return {};
    }

    std::string msg {monitor->readBuffer.data() + startMarkerIndex, packlength};
    
    // print message
    std::cout << "uncut: \t" << msg << std::endl;
    // XXX: check that this is checksumming the correct bytes
    std::string package = msg.substr(1, packlength - 2 /*for '*' and '$' */ - 2 /* for '$cs' */); // between $ and *
    std::cout << "cut: \t" << package << std::endl;
    std::string incomingChecksum {msg.substr(packlength - 2, 2)};
    std::cout << "cs: \t" << incomingChecksum << std::endl;
    // validate the checksum
    const std::string calcCheckSum = checksumHex(package); 

    if(calcCheckSum != incomingChecksum) {
        std::cout << "CHECKSUMS DO NOT MATCH, expected:" << calcCheckSum << "; received: " << incomingChecksum << std::endl;
        // remove broken packet
        monitor->readBuffer.erase(monitor->readBuffer.begin(), monitor->readBuffer.begin() + startMarkerIndex + packlength);
        
        return {};
    }

    // handle packet based on msg id
    std::string msgID = msg.substr(1, 5);

    std::vector<char> returnVal{};
    returnVal.resize(packlength);
    memcpy(returnVal.data(), monitor->readBuffer.data(), packlength);
    // remove packet data from buffer
    monitor->readBuffer.erase(monitor->readBuffer.begin(), monitor->readBuffer.begin() + startMarkerIndex + packlength);

    if(msgID == "RXHRB") {
        std::cout << "HEART BEAT..." << std::endl; 
    } else {
        std::cout << "other... " << msgID << std::endl;
        return returnVal;
    }
    return {}; // dont return heartbeats
    
}