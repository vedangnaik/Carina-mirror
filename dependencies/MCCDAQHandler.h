#pragma once

#include <uldaq.h>

class MCCDAQHandler {
public:
    explicit MCCDAQHandler(DaqDeviceDescriptor d);
    ~MCCDAQHandler();

    std::string uniqueID;
    const DaqDeviceHandle handle;
    const long long numChannels;
    const unsigned int samplesPerChannel = 100;
    const std::unique_ptr<double[]> dataBuffer;
private:
    const DaqDeviceDescriptor d;
};