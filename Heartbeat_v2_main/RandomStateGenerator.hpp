#ifndef RANDOMSTATEGENERATOR_HPP
#define RANDOMSTATEGENERATOR_HPP

#include "RXState.hpp"
#include <thread>


namespace roboseals::RX_Message::test
{

    /**
     * @class RandomStateGenerator
     * @author John Bures
     * @date 17/11/2022
     * @file RandomStateGenerator.hpp
     * @brief Generates random data to be sent to the Technical Director's network. This class provides a
     * single function, run(), that when executed, manipulates the state randomly. Since completion requires
     * a change of boats state, this class also runs a thread that, after a few seconds, changes the USV state. 
     */
class RandomStateGenerator
{
public:
    RandomStateGenerator(roboseals::RX_Message::RXSystemState &state);
    ~RandomStateGenerator();

    protected:
    void run(roboseals::RX_Message::RXSystemState &state);
private:
    std::unique_ptr<std::thread> _changeDriver;
};

}

#endif // RANDOMSTATEGENERATOR_HPP
