#pragma once

#include "easylogging++.h"
#include "AbstractDAQ.h"
#include <random>

class DummyDAQ : public AbstractDAQ
{
public:
    DummyDAQ(std::string deviceID, unsigned int numChannels);
    void startAcquisition() override;
    void stopAcquisition() override;
    std::vector<double> getLatestData() override;
};

