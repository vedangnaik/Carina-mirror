#pragma once

#include "easylogging++.h"
#include "Sensor.h"
#include <random>

class DummyDAQ : public Sensor
{
public:
    DummyDAQ(const std::string id);
    void startAcquisition() override;
    void stopAcquisition() override;
    double getLatestData() override;
private:
    std::mt19937 gen;
    std::uniform_real_distribution<> dis;
};
