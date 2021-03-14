#ifdef WIRINGPI_AVAILABLE

#pragma once

#include <wiringPiI2C.h>
#include "AbstractDAQ.h"
#include "easylogging++.h"
#include <cmath>
#include <chrono>
#include <thread>

class I2CDAQ : public AbstractDAQ
{
public:
    I2CDAQ(const std::string id, const unsigned int numChannels, const char I2CAddress);
    void startAcquisition() override;
    void stopAcquisition() override;
    std::vector<double> getLatestData() override;
private:
    int fd;  // -1 if device not found
    const char I2CAddress;
};

#endif
