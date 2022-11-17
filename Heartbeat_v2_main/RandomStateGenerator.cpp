#include "RandomStateGenerator.hpp"
#include <memory>
#include <iostream>
#include <thread>
#include <ctime>


roboseals::RX_Message::test::RandomStateGenerator::RandomStateGenerator(roboseals::RX_Message::RXSystemState & state)
{
    this->_changeDriver = std::make_unique<std::thread>(&roboseals::RX_Message::test::RandomStateGenerator::run, this, std::ref(state));
}

roboseals::RX_Message::test::RandomStateGenerator::~RandomStateGenerator()
{
    
}

void changeUAVStatus(roboseals::RX_Message::RXSystemState & state);


void roboseals::RX_Message::test::RandomStateGenerator::run(roboseals::RX_Message::RXSystemState & state)
{
    
    using namespace std::chrono_literals;
    
    // initial lat/long
    double originLong = 150.67114;
    double originLat = -33.723143;
    
    // begins thread which, after delay, changes usv status
    std::thread delayUAVChange{&changeUAVStatus, std::ref(state)};
    
    state.killed = false;
    state.latitude = originLat;
    state.longitude = originLong;
    state.uavStatus = roboseals::RX_Message::UAV_Status::Stowed;
    state.systemMode = roboseals::RX_Message::SystemMode::Killed;

    srand((unsigned int) time (NULL)); //activates the generator
    
    while (true) {
        // randomise latitude
        auto r = ((double) rand() / (RAND_MAX))/100;
        state.latitude.store(originLat + (r - r/2.0));
        
        // randomise longitude
        auto r2 = ((double) rand() / (RAND_MAX))/100;
        state.longitude.store(originLong + (r2 - r2/2.0));
        
        // allow other threads to run; only reports state to tech director at 1 hz, no need to change state
        // more than that; 0.5 hz here for safety.
        std::this_thread::sleep_for(500ms);
    }
    
}

// used to change the usv state after three seconds
void changeUAVStatus(roboseals::RX_Message::RXSystemState & state) {
    std::cout  << "Starting to wait for system state change..." << std::endl;
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(3000ms);
    state.systemMode = roboseals::RX_Message::SystemMode::Autonomous;
    std::cout << "Changed system state!" << std::endl;
}