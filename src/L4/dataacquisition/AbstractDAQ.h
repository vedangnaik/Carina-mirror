#pragma once

#include <map>
#include <vector>
#include <string>
#include <numeric>
#include <cmath>

class AbstractDAQ {
public:
    AbstractDAQ(const std::string deviceID, const unsigned int numChannels, const std::pair<std::array<double, 5>, std::array<double, 5>> calibrationPoints);
    virtual void startAcquisition() = 0;
    virtual void stopAcquisition() = 0;
    virtual std::vector<double> getLatestData() = 0;
    virtual ~AbstractDAQ() {};
    void calibrate();
    void calibrate(std::pair<std::array<double, 5>, std::array<double, 5>> calibrationPoints);

    const std::string deviceID;
    const unsigned int numChannels;
    std::pair<std::array<double, 5>, std::array<double, 5>> calibrationPoints;
protected:
    double slope;
    double y_intercept;
};
