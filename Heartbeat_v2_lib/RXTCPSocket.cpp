#include "RXTCPSocket.hpp"
#include <string>
#include <iostream>
#include <cstring>
#include "RXMessageBuilder.hpp"

const std::vector<char> roboseals::RX_Message::RXTCPSocket::popBytes() {
    //std::cout << "interpret bytes... (" << std::to_string(readBuffer().size()) << " bytes)" << std::endl;
    bool headerFound = false;

    int ptrIndex = 0;
    for(; ptrIndex < readBuffer().size()-1; ptrIndex++) {
        // search for packet start
        if(((uint8_t)readBuffer()[ptrIndex]) == (uint8_t) '$') {
            headerFound = true;
            break;
        }
    }

    if(!headerFound) {
        // if there is no packet start code, the packet is not found then do not read
        return {};
    }
    
    const auto startMarkerIndex = ptrIndex;

    // get the first '$' marker in the buffered bytes
    auto ptr = readBuffer().begin() + startMarkerIndex;

    size_t packlength = 0;
    bool foundEndMarker = false;
    for(auto itr = readBuffer().begin() + startMarkerIndex; itr < readBuffer().end(); itr++) {
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
    } else if (startMarkerIndex + packlength > readBuffer().size()) {
        // buffer does not contain msg (including checksum, etc.)
        return {};
    }

    std::string msg {readBuffer().data() + startMarkerIndex, packlength};
    
    // print message
    std::cout << "uncut: " << msg << std::endl;
    msg = msg.substr(startMarkerIndex, startMarkerIndex + packlength);
    std::cout << "cut: " << msg << std::endl;

    // validate the checksum
    const uint16_t calcCheckSum = checksum(msg.substr(0, packlength-2)); // XXX: check that this is checksumming the correct bytes
    const uint16_t incomingChecksum = *(ptr + packlength-2); // XXX: check this is the correct byte

    if(calcCheckSum != incomingChecksum) {
        std::cout << "CHECKSUMS DO NOT MATCH, expected:" << std::to_string(calcCheckSum) << "; received: " << std::to_string(incomingChecksum) << std::endl;
        // remove broken packet
        readBuffer().erase(readBuffer().begin(), readBuffer().begin() + startMarkerIndex + packlength);
        return {};
    }

    // handle packet based on msg id
    std::string msgID {*(ptr + 1)};
    msgID = msgID.substr(5);

    std::vector<char> returnVal{};
    returnVal.resize(ptrIndex + packlength);
    memcpy(returnVal.data(), readBuffer().data(), ptrIndex + packlength);
    // remove packet data from buffer
    readBuffer().erase(readBuffer().begin(), readBuffer().begin() + startMarkerIndex + packlength);

    if(msgID == "RXHRB") {
        std::cout << "HEART BEAT..." << std::endl; 
    } else {
        std::cout << "other... " << msgID << std::endl;
        return returnVal;
    }
    return {}; // dont return heartbeats
    
}