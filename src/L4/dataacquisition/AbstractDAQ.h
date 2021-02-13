#pragma once

#include <map>
#include <vector>
#include <string>
#include <numeric>
#include <cmath>

class AbstractDAQ {
public:
    AbstractDAQ(const std::string deviceID, const unsigned int numChannels, const std::vector<std::pair<std::array<double, 5>, std::array<double, 5>>> calibrationPoints);
    virtual void startAcquisition() = 0;
    virtual void stopAcquisition() = 0;
    virtual std::vector<double> getLatestData() = 0;
    virtual ~AbstractDAQ() {};

    const std::string deviceID;
    const unsigned int numChannels;
protected:
    std::vector<std::pair<double, double>> slopesAndIntercepts;
private:
    void calibrate(std::vector<std::pair<std::array<double, 5>, std::array<double, 5>>> calibrationPoints);
};
