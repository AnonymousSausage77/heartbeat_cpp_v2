#ifndef TESTC_HPP
#define TESTC_HPP

extern "C" {
#include "can_control.h"
}

void commstest() {
    
    
    int *ps; 
    
    auto c = InitialiseCAN(can0, ps);
    
    
}

#endif