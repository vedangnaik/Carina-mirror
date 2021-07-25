#pragma once

#include <map>
#include <vector>
#include <string>
#include <numeric>
#include <cmath>
#include <array>
#include <utility>

class AbstractDAQ {
public:
    AbstractDAQ(const std::string deviceID, const unsigned int numChannels);
    virtual void startAcquisition() = 0;
    virtual void stopAcquisition() = 0;
    virtual std::vector<double> getLatestData() = 0;
    virtual ~AbstractDAQ() {};
    void calibrate();

    const std::string deviceID;
    const unsigned int numChannels;
    std::vector<std::array<std::pair<double, double>, 5>> calibrationPoints;
protected:
    std::vector<std::pair<double, double>> slopesAndIntercepts;
};
