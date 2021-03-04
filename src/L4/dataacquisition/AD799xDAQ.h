// #ifdef WIRINGPI_AVAILABLE

#pragma once

#include <wiringPiI2C.h>
#include "AbstractDAQ.h"
#include "easylogging++.h"
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include <chrono>
#include <thread>

class AD799xDAQ : public AbstractDAQ
{
public:
    AD799xDAQ(std::string id, unsigned int numChannels, char I2CAddress);
    void startAcquisition() override;
    void stopAcquisition() override;
    std::vector<double> getLatestData() override;
private:
    private int fd;  // -1 if device not found
    const char I2CAddress;
};

// #endif
