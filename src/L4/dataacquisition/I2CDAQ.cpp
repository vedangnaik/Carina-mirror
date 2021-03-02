#ifdef WIRINGPI_AVAILABLE

#include "I2CDAQ.h"

I2CDAQ::I2CDAQ(const std::string id, const unsigned int numChannels, const std::vector<std::pair<std::array<double, 5>, std::array<double, 5>>> calibrationPoints, const char I2CAddress)
    : AbstractDAQ(id, numChannels, calibrationPoints), I2CAddress{I2CAddress}
{

}

void
I2CDAQ::startAcquisition()
{

}

void
I2CDAQ::stopAcquisition()
{

}

std::vector<double>
I2CDAQ::getLatestData()
{
    return {};
}

#endif
