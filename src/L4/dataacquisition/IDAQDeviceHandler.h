#pragma once

#include <map>
#include <vector>
#include <string>

class IDAQDeviceHandler {
public:
    virtual void startAcquisition() = 0;
    virtual void stopAcquisition() = 0;
    virtual std::map<unsigned int, std::vector<double>> getLatestData() = 0;
    virtual ~IDAQDeviceHandler() {};
    std::string deviceID;
};
