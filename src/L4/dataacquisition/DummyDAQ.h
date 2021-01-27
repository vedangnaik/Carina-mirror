#pragma once

#include "easylogging++.h"
#include "AbstractDAQ.h"
#include <random>

class DummyDAQ : public AbstractDAQ
{
public:
    DummyDAQ(const std::string deviceID, const unsigned int numChannels, const std::pair<std::vector<double>, std::vector<double>> calibrationPoints);
    void startAcquisition() override;
    void stopAcquisition() override;
    std::vector<double> getLatestData() override;
};
