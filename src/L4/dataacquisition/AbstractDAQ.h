#pragma once

#include <map>
#include <vector>
#include <string>
#include <numeric>
#include <cmath>

class AbstractDAQ {
public:
    AbstractDAQ(const std::string deviceID, const unsigned int numChannels, const std::pair<std::vector<double>, std::vector<double> > calibrationPoints);
    virtual void startAcquisition() = 0;
    virtual void stopAcquisition() = 0;
    virtual std::vector<double> getLatestData() = 0;
    virtual ~AbstractDAQ() {};
    const std::string deviceID;
    const unsigned int numChannels;
protected:
    double slope;
    double y_intercept;
};
