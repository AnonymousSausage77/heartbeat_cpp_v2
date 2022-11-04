#include "RXMessageBuilder.hpp"
#include <sstream>
#include <iomanip>

using namespace roboseals::RX_Message;

RXMessageBuilder::RXMessageBuilder()
{

}

RXMessageBuilder& RXMessageBuilder::initialise(const std::string &id, const std::string &date, const std::string &time)
{
    this->_message = "";
    this->append(id);
    this->append(date);
    this->append(time);
    return *this;
}

RXMessageBuilder &RXMessageBuilder::append(const std::string &s)
{
    if (this->_message != "") {
        _message += ",";
    }
    _message.append(s);
    return *this;
}

RXMessageBuilder &RXMessageBuilder::append(const long i)
{
    std::string s = std::to_string(i);
    append(s);
    return *this;
}


RXMessageBuilder &RXMessageBuilder::coords(const double latitude, const double longitude)
{
    std::stringstream ss;

    // latitude
    append(latitude, 5);
    append(latitude < 0 ? "S":"N");

    // clear
    ss.str(std::string());
    ss.clear();

    // longitude
    append(longitude, 5);
    append(longitude < 0 ? "E":"W");

    return *this;
}

RXMessageBuilder &RXMessageBuilder::append(const double value, const int precision)
{

    std::stringstream ss;
    ss<< std::fixed << std::setprecision(precision) << value;
    const std::string dString ( ss.str() );
    append(dString);

    return *this;
}



uint16_t RXMessageBuilder::checksumOf(std::string &s) const
{
    return RX_Message::checksum(s);
}

std::string RXMessageBuilder::checksumHex(std::string &s) const
{
    return RX_Message::checksumHex(s);
}

std::string RXMessageBuilder::get()
{
    auto cs = checksumHex(this->_message);
    std::string val = "$" + this->_message + "*" + cs + "\n";
        // assumes "\n" should hopefully be transformed into <cr><lf>
        // TODO: verify assumption
    this->_message = "";
    return val;
}
