#ifdef WIRINGPI_AVAILABLE

#pragma once

//#include <wiringPiI2C.h>
#include "AbstractDAQ.h"

class I2CDAQ : public AbstractDAQ
{
public:
    I2CDAQ(const std::string id, const unsigned int numChannels, const std::vector<std::pair<std::array<double, 5>, std::array<double, 5>>> calibrationPoints, const char I2CAddress);
    void startAcquisition() override;
    void stopAcquisition() override;
    std::vector<double> getLatestData() override;
private:
    const char I2CAddress;
};

#endif
