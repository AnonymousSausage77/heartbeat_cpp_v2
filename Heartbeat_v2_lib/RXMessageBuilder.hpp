#ifndef RXMESSAGESTRINGBUILDER_H
#define RXMESSAGESTRINGBUILDER_H

#include <string>
#include <stdio.h>
#include <sstream>

namespace roboseals::RX_Message {

    /**
     * @class RXMessageBuilder
     * @author John Bures
     * @date 11/07/22
     * @file RXMessageBuilder.hpp
     * @brief Builder pattern, used to construct messages for the Technical Director's network.
     */
class RXMessageBuilder
{
public:
    RXMessageBuilder();
    RXMessageBuilder &initialise(const std::string& id, const std::string& date, const std::string& time);
    RXMessageBuilder &append(const std::string& s);
    RXMessageBuilder &append(const long i);
    RXMessageBuilder &coords(const double latitude, const double longitude);
    RXMessageBuilder &append(const double value, const int precision);

    std::string get();
private:
    std::string _message = "";

    uint16_t checksumOf(std::string &s) const;
    std::string checksumHex(std::string &s) const;
};


inline uint16_t checksum(const std::string &s)
{
    
    uint16_t cs = s.at(0);
    for (auto itr = s.begin() + 1; itr < s.end(); itr++) {
        cs ^= *itr;
    }
    return cs;
}

inline std::string checksumHex(const std::string &s)
{
    auto cs = checksum(s);

    // convert to hex
    std::stringstream ss;
    ss<< std::hex << cs;
    std::string res ( ss.str() );

    return res;
}

}
#endif // RXMESSAGESTRINGBUILDER_H
