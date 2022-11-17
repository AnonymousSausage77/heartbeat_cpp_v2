#ifndef ROOCOMMS_PAYLOAD_H
#define ROOCOMMS_PAYLOAD_H

#include <stdint.h>

namespace roboseals::RooCOMMS {

// not a packet
struct UAV_OBJECT_DETECTION {
    char targetObject; // 'R' or 'N' (represents a 'flat white panel with "R" or "N"' on it)
    long latitude;
    long longitude;
};
    
namespace Packet {

struct GPS_UPDATE { // sent regularly from the gps system
    long latitude;
    long longitude;
};


struct USV_UPDATE { // sent regularly from the rio
    uint8_t systemMode; // 1="remote operated",2="autonomous",3="killed"
    uint8_t uavStatus; // 1="stowed",2="deployed",3="faulted"
};


/*
 * Below are task specific and are only sent when completing specific tasks.
 */
 
struct GATE_REPORT {
    uint8_t entranceGate; // 1, 2, or 3
    uint8_t exitGate; // 1, 2, or 3
};
struct PATH_SUCCESS {
    uint8_t isFinished; // 1="in progress",  2="completed"
};
struct WILDLIFE_REPORT {
    uint8_t numberDetected; // number of wildlife items detected (Max 3)
    char *wildlifeObjects; // array of characters 'P'=playpus, 'C'=Crocodile, or 'T'=Turtle

    /*
     * length(wildlifeObjects) == numberDetected, for example:
     *  numberDetected: 2, wildlifeObjects: ['C', 'T']
    */

};
struct SCAN_CODE_REPORT {
    char *pattern; // 3 character c-string of 'R'=red,'G'=green,'B'=blue
        // eg. ['R', 'B', 'G']
};
struct DETECT_DOCK_REPORT {
    char colour; // colour of the docking bay ('R', 'B', or 'G')
    uint8_t amsStatus; // 1=docking, 2=complete
    
};
struct FIND_FLING_REPORT {
    char faceColour; // ('R','G', or, 'B'), colour of the face being targeted
    uint8_t amsStatus; // 1=scanning, 2=flinging
};
struct UAV_REPLENISHMENT_REPORT {
    uint8_t uavStatus; // 1=stowed, 2=deployed, 3=faulted
    uint8_t itemStatus; // 0=not yet picked up, 1=picked up, 2=delivered
};

struct UAV_SEARCH_REPORT {
    UAV_OBJECT_DETECTION obj1;
    UAV_OBJECT_DETECTION obj2;
    uint8_t uavStatus; // 1=manual, 2=autonomous, 3=faulted
};

}

}


#endif // ROOCOMMS_PAYLOAD_H
