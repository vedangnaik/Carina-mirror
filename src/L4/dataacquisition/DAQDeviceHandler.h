#pragma once

#include <map>
#include <vector>
#include <string>

class DAQDeviceHandler {
public:
    virtual void startAcquisition() = 0;
    virtual void stopAcquisition() = 0;
    virtual std::map<unsigned int, std::vector<double>> getLatestData() = 0;
    virtual ~DAQDeviceHandler() {};
    std::string deviceID;
};
