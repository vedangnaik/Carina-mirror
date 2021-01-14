#pragma once

#include <map>
#include <vector>
#include <string>

class AbstractDAQDeviceHandler {
public:
    AbstractDAQDeviceHandler(std::string deviceID, unsigned int numChannels) : deviceID{deviceID}, numChannels{numChannels} {};
    virtual void startAcquisition() = 0;
    virtual void stopAcquisition() = 0;
    virtual std::vector<double> getLatestData() = 0;
    virtual ~AbstractDAQDeviceHandler() {};
    const std::string deviceID;
    const unsigned int numChannels;
};
