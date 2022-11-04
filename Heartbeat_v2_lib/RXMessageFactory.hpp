#ifndef RXMESSAGEFACTORY_H
#define RXMESSAGEFACTORY_H
#include <string>
#include "RXState.hpp"

namespace roboseals::RX_Message {

class RXMessageBuilder;


class RXMessageFactory
{
public:
    RXMessageFactory(const std::string &teamID);
    ~RXMessageFactory();
    std::string gateMessage(const std::string& entrance, const std::string &exit);
    std::string heartbeatMessage(const double longitude, const double lat, const int systemMode, const int uavStatus);
    std::string followPathMessage(const bool isFinished);
    std::string wildlifeEncounterMessage(const uint8_t number, const WildlifeType *targets);
    std::string scanCodeMessage(const std::string &lightPattern);
    std::string detectDockMessage(const Colour &colour, const int amsStatus);
    std::string findAndFlingMessage(const Colour &colour, const int amsStatus);
    std::string uavReplenishmentMessage(const int uavStatus, const int itemStatus);
    std::string uavSearchReportMessage(const SearchObjectDetails &o1, const SearchObjectDetails &o2, const int uavStatus);
private:
    RXMessageBuilder &header(const std::string &msgID);

    const std::string _teamID;
    RXMessageBuilder* _sBuilder;
};

}
#endif // RXMESSAGEFACTORY_H
