#pragma once

#include <uldaq.h>
#include <string>
#include <memory>
#include "easylogging++.h"

class MCCDAQHandler {
public:
    explicit MCCDAQHandler(DaqDeviceDescriptor d);
    ~MCCDAQHandler();

    std::string uniqueID;
    DaqDeviceHandle handle;
    long long numChannels;
    const unsigned int samplesPerChannel = 100;
    std::unique_ptr<double[]> dataBuffer;
private:
    const DaqDeviceDescriptor d;
    double rate = 100.0;
};