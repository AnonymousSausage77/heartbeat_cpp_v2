#ifndef RCCLIENTADAPTOR_HPP
#define RCCLIENTADAPTOR_HPP

#include "AbstractRCClientAdaptor.hpp"

using namespace roboseals::RooCOMMS;

namespace roboseals::RooCOMMS::test
{

class RCClientAdaptor : public AbstractRCClientAdaptor
{
public:
    RCClientAdaptor();
    ~RCClientAdaptor();
    void updateState(roboseals::RX_Message::RXSystemState &state) override;
    std::vector<std::string> fetchMessagesToBeSent(roboseals::RX_Message::RXMessageFactory &factory) override;
private:
    void runGPSPackets();
    void runSuccessPacket();

};

}

#endif // RCCLIENTADAPTOR_HPP
