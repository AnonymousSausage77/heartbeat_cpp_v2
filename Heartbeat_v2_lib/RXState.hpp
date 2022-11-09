#ifndef RXSYSTEMSTATE_H
#define RXSYSTEMSTATE_H

#include <stdint.h>
#include <string>

namespace roboseals::RX_Message {
    
    
enum class WildlifeType : char {
    platypus = 'P', crocodile = 'C', turtle = 'T'
};

enum class Colour : char {
    red = 'R', green = 'G', blue = 'B'
};

inline std::string& operator<<(std::string &s, const WildlifeType &type) {
    std::string c{1, (char)type};
    return s.append(c);
}

inline std::string& operator<<(std::string &s, const Colour &type) {
    std::string c{1, (char)type};
    return s.append(c);
}



struct SearchObjectDetails {
    std::string id; // "R" or "N"
    double latitude;
    double longitude;
};

enum class UAV_Status : unsigned int {
    Stowed = 1,
    Deployed = 2,
    Faulted = 3
};

enum class SystemMode : unsigned int {
    Remote_Operated = 1,
    Autonomous = 2,
    Killed = 3
};

/**
 * @class RXSystemState
 * @author John Bures 
 * @date 11/07/22
 * @file RXState.hpp
 * @brief The internal model of the Heartbeat system. Each heartbeat message is built from the data held within an instantiation of this
 *  struct.
 */
struct RXSystemState {
    // boat state
    UAV_Status uavStatus;
    SystemMode systemMode;
    int64_t statusUpdateTimeStamp;
    bool killed; // if true, heartbeat system will stop

    // gps
    double longitude;
    double latitude;
    int64_t gpsUpdateTimeStamp; // last time of gps update
};

}

#endif // RXSYSTEMSTATE_H
