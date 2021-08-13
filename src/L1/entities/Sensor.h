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
    Sensor(const std::string id, const unsigned int numChannels);
    virtual void startAcquisition() = 0;
    virtual void stopAcquisition() = 0;
    virtual std::vector<double> getLatestData() = 0;
    virtual ~Sensor() {}
    
    void calibrate();

    const std::string id;
    float latestValue;
    const unsigned int numChannels;
    std::vector<std::array<std::pair<double, double>, 5>> calibrationPoints;
protected:
    std::vector<std::pair<double, double>> slopesAndIntercepts;
};
