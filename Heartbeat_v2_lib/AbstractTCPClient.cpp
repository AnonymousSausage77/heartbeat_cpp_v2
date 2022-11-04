#include "AbstractTCPClient.hpp"
#include <iostream>

roboseals::AbstractTCPClient::AbstractTCPClient()
{
    std::cout << "TCP Client" << std::endl;
}

roboseals::AbstractTCPClient::~AbstractTCPClient()
{
}


/*
void roboseals::AbstractTCPClient::addDataReceivedListener(long (*listener)(const uint8_t*, const uint32_t))
{
    this->_dataReceivedListeners.push_back(listener);
}


void roboseals::AbstractTCPClient::updateDataReceivedListeners(const uint8_t * bytes, const uint32_t bsize) const
{
    for (auto obs : this->_dataReceivedListeners) {
        obs(bytes, bsize);
    }
}*/