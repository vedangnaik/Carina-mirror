#pragma once

#include "easylogging++.h"
#include "AbstractDAQ.h"
#include <random>

class DummyDAQ : public Sensor
{
public:
    DummyDAQ(const std::string deviceID, const unsigned int numChannels);
    void startAcquisition() override;
    void stopAcquisition() override;
    std::vector<double> getLatestData() override;
private:
    std::mt19937 gen;
    std::uniform_real_distribution<> dis;
};
