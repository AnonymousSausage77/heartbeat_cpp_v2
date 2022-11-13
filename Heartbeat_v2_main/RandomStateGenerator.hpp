#ifndef RANDOMSTATEGENERATOR_HPP
#define RANDOMSTATEGENERATOR_HPP

#include "RXState.hpp"
#include <thread>

namespace roboseals::RX_Message::test
{

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
