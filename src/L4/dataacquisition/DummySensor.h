#pragma once

#include "easylogging++.h"
#include "Sensor.h"
#include <random>

class DummySensor : public Sensor
{
public:
    DummySensor(const std::string& id, const std::vector<std::pair<double, double>>& calibrationPoints);
    void startAcquisition() override;
    void stopAcquisition() override;
    double getLatestData() override;
private:
    std::mt19937 gen;
    std::uniform_real_distribution<> dis;
};
