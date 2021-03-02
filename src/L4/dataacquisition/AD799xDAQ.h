// #ifdef WIRINGPI_AVAILABLE

#pragma once

#include <wiringPiI2C.h>
#include "AbstractDAQ.h"
#include "easylogging++.h"
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>

class AD799xDAQ : public AbstractDAQ
{
public:
    AD799xDAQ(std::string id, unsigned int numChannels, char I2CAddress, std::string I2CPath);
    void startAcquisition() override;
    void stopAcquisition() override;
    std::vector<double> getLatestData() override;
private:
    const std::string I2CPath;
    const char I2CAddress;
    std::ifstream I2CPort;
};

// #endif
