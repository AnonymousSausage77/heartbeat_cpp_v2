#include "RXMessageFactory.hpp"
#include "RXMessageBuilder.hpp"
#include "RXTimeFormat.hpp"

using namespace roboseals;
using namespace roboseals::RX_Message;

RX_Message::RXMessageFactory::RXMessageFactory(const std::string& teamID) : _teamID(teamID)
{
   this->_sBuilder = new RXMessageBuilder();

}

RX_Message::RXMessageFactory::~RXMessageFactory()
{
    delete this->_sBuilder;
    this->_sBuilder = nullptr;
}

std::string RX_Message::RXMessageFactory::gateMessage(const std::string &entrance, const std::string &exit)
{
    auto builder = this->header("RXGAT")
            .append(this->_teamID)
            .append(entrance)
            .append(exit);
    return builder.get();
}

std::string RX_Message::RXMessageFactory::heartbeatMessage(const double latitude, const double longitude, const int systemMode, const int uavStatus)
{
    return this->header("RXHRB")
            .coords(latitude, longitude) // TODO: these should have different formattings (different number of decimals)
            .append(this->_teamID)
            .append(systemMode)
            .append(uavStatus)
            .get();
}

std::string RX_Message::RXMessageFactory::followPathMessage(const bool isFinished)
{
    return this->header("RXPTH")
            .append(this->_teamID)
            .append(isFinished ? "2" : "1")
            .get();
}

std::string RX_Message::RXMessageFactory::wildlifeEncounterMessage(const uint8_t number, const WildlifeType *targets)
{
    this->header("RXENC").append(number);
    for(uint8_t i = 0; i < number; i++) {
        std::string s {};
        s << *(targets + i);
        _sBuilder->append(s);
    }
    return this->_sBuilder->get();
}

std::string RX_Message::RXMessageFactory::scanCodeMessage(const std::string &lightPattern)
{
    return this->header("RXCOD")
            .append(this->_teamID)
            .append(lightPattern)
            .get();
}

std::string RX_Message::RXMessageFactory::detectDockMessage(const Colour &colour, const int amsStatus)
{
    std::string c;
    c << colour;

    return this->header("RXDOK")
            .append(this->_teamID)
            .append(c)
            .append(amsStatus)
            .get();
}

std::string RX_Message::RXMessageFactory::findAndFlingMessage(const Colour &colour, const int amsStatus)
{

    std::string c;
    c << colour;

    return this->header("RXFLG")
            .append(this->_teamID)
            .append(c)
            .append(amsStatus)
            .get();
}

std::string RX_Message::RXMessageFactory::uavReplenishmentMessage(const int uavStatus, const int itemStatus)
{
    return this->header("RXUAV")
            .append(this->_teamID)
            .append(uavStatus)
            .append(itemStatus)
            .get();
}

std::string RX_Message::RXMessageFactory::uavSearchReportMessage(const SearchObjectDetails &o1, const SearchObjectDetails &o2, const int uavStatus)
{
    return this->header("RXSAR")
            .append(o1.id).coords(o1.latitude, o1.longitude)
            .append(o2.id).coords(o2.latitude, o2.longitude)
            .append(this->_teamID)
            .append(uavStatus)
            .get();
}



RX_Message::RXMessageBuilder &RX_Message::RXMessageFactory::header(const std::string &msgID)
{
    const std::string timeString = roboseals::RX_Message::currentDateTime();
    const std::string date = timeString.substr(0, timeString.find(","));
    const std::string time = timeString.substr(timeString.find(",")+1);
    this->_sBuilder->initialise(msgID, date, time);
    return *_sBuilder;
}
