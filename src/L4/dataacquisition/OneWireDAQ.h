#pragma once

#include "AbstractDAQ.h"
#include "easylogging++.h"
#include <string>
#include <cmath>

// default GPIO raspberry pi pin for one wire is pin 4
class OneWireDAQ : public AbstractDAQ
{
public:
    OneWireDAQ(const std::string deviceID, const unsigned int numChannels, std::string devicePath);
    void startAcquisition() override;
    void stopAcquisition() override;
    std::vector<double> getLatestData() override;

private:
    std::string devicePath;
};

