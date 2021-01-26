//#ifdef WIRINGPI_AVAILABLE

#include "AD799xDAQ.h"

AD799xDAQ::AD799xDAQ(std::string id, unsigned int numChannels, char I2CAddress)
    : AbstractDAQ(id, numChannels), I2CAddress{I2CAddress}
{

}

void
AD799xDAQ::startAcquisition()
{

}

void
AD799xDAQ::stopAcquisition()
{

}

std::vector<double>
AD799xDAQ::getLatestData()
{
    return {};
}

//#endif
