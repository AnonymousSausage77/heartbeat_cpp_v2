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

struct RXSystemState {
    UAV_Status uavStatus;
    SystemMode systemMode;
    int64_t statusUpdateTimeStamp;

    double longitude;
    double latitude;
    int64_t gpsUpdateTimeStamp;
};

}

#endif // RXSYSTEMSTATE_H
