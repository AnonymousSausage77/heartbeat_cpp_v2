#ifndef ABSTRACTRCCLIENTADAPTOR_HPP
#define ABSTRACTRCCLIENTADAPTOR_HPP

#include "RXState.hpp"
#include <vector>
#include <string>
#include "RXMessageFactory.hpp"

namespace roboseals::RooCOMMS {
    
    /**
     * @class AbstractRCClientAdaptor
     * @author John Bures
     * @date 11/07/22
     * @file AbstractRCClientAdaptor.hpp
     * @brief Interface for the RooComms CANBUS network. This interface hides the implementation of fetching messages from the CANBUS network
     *  by offering functions that simply retrieve the required data.
     */
    class AbstractRCClientAdaptor {
    public:
        /**
         * @brief Updates a state model with the data that the AbstractRCClientAdaptor has received from the CANBUS network. This function is
         * called once every tick with the state object being passed in. If new GPS etc. data has been received since the last call, it is injected
         * into the &state object, otherwise nothing changes.
         * @param state Internal model of the current gps/mode of the boat
         */
        virtual void updateState(roboseals::RX_Message::RXSystemState &state) = 0;
        /**
         * @brief Fetches a vector of messages that need to be forwarded to the Technical Director's network. This is specific to structs 
         * "success messages" not heartbeat messages -- success messages being those packets which are only sent for a particular task, not
         * for the heartbeat (which is meant to be sent every second)
         * @param factory   A message factory ref used to build the packets for sending to the Technical Director's network
         * @return A vector of messages that need to be forwarded
         */
        virtual std::vector<std::string> fetchMessagesToBeSent(roboseals::RX_Message::RXMessageFactory &factory) = 0;
    };
}

#endif // ABSTRACTRCCLIENTADAPTOR_HPP