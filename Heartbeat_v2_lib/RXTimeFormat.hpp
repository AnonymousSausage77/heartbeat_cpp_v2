#ifndef TIMECONVERTER_H
#define TIMECONVERTER_H

#include <chrono>

#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>
namespace roboseals::RX_Message {
    
/**
 * @brief Gets the current time as a string in the format of "ddMMyy,HHmmss"
 */
inline const std::string currentDateTime() {
    time_t now = time(0); 
    struct tm tstruct;
    char buf[80]; // put the resultant string here
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%d%m%y,%H%M%S", &tstruct);

    return buf;
}

}

#endif // TIMECONVERTER_H
