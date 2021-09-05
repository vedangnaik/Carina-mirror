#pragma once

#include "AbstractDAQ.h"
#include "easylogging++.h"
#include <string>
#include <cmath>

// default GPIO raspberry pi pin for one wire is pin 4
class OneWireDAQ : public AbstractDAQ
{
public:
    OneWireDAQ(const std::string deviceID, int gpioPin, const unsigned int numChannels);
    void startAcquisition() override;
    void stopAcquisition() override;
    std::vector<double> getLatestData() override;

private:
    int gpioPin;
    std::string w1_file;
};

