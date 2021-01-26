#ifdef WIRINGPI_AVAILABLE

#pragma once

#include <wiringPiI2C.h>
#include "AbstractDAQ.h"

class AD799xDAQ : public AbstractDAQ
{
public:
    AD799xDAQ(std::string id, unsigned int numChannels, char I2CAddress);
    void startAcquisition() override;
    void stopAcquisition() override;
    std::vector<double> getLatestData() override;
private:
    const char I2CAddress;
};

#endif
