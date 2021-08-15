#pragma once

#include <map>
#include <vector>
#include <string>
#include <numeric>
#include <cmath>
#include <array>
#include <utility>

enum class SensorOption {

};

struct SensorCheck {
    int a;
    int b;
};

class Sensor {
public:
    Sensor(const std::string& id, const std::array<std::pair<double, double>, 5>& calibrationPoints);
    virtual void startAcquisition() = 0;
    virtual void stopAcquisition() = 0;
    virtual double getLatestData() = 0;
    virtual ~Sensor() = default;
    
    void calibrate();

    const std::string id;
    double latestValue = 0.0;
    std::array<std::pair<double, double>, 5> calibrationPoints;
protected:
    double slope = 1.0;
    double intercept = 0.0;
};
