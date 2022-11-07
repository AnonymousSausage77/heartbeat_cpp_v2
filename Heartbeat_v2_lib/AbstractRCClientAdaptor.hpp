#ifndef ABSTRACTRCCLIENTADAPTOR_HPP
#define ABSTRACTRCCLIENTADAPTOR_HPP

#include "RXState.hpp"
#include <vector>
#include <string>
#include "RXMessageFactory.hpp"

namespace roboseals::RooCOMMS {
    
    class AbstractRCClientAdaptor {
    public:
        virtual void updateState(roboseals::RX_Message::RXSystemState &state) = 0;
        virtual std::vector<std::string> fetchMessagesToBeSent(roboseals::RX_Message::RXMessageFactory &factory) = 0;
        virtual bool shouldStop() const = 0;
    };
}

#endif // ABSTRACTRCCLIENTADAPTOR_HPP