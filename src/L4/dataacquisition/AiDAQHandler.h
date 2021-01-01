#ifdef USE_ULDAQ

#pragma once
#include <uldaq.h>
#include <stdlib.h>
#include <map>
#include <vector>
#include "DAQDeviceHandler.h"
#include <memory>
#include <iostream>

/**
 * @brief The AiDAQHandler class handles an analog input DAQ device
 */
class AiDAQHandler : public DAQDeviceHandler {
public:
    AiDAQHandler(DaqDeviceHandle handle, unsigned int numChannels, Range voltageRange);
    ~AiDAQHandler();
    void startAcquisition() override;
    std::map<unsigned int, std::vector<double>> getLatestData() override;
    void stopAcquisition() override;
private:
    // config vars
    const DaqDeviceHandle handle;
    const unsigned int samplesPerChannel = 100;
    const unsigned int numChannels;
    const Range voltageRange;
    double rate = 100; // must be non-const for uldaq.h :(
    const AiInputMode aiim = AI_SINGLE_ENDED;
    const ScanOption so = SO_CONTINUOUS;
    const AInScanFlag aisf = AINSCAN_FF_DEFAULT;
    // status vars
    std::unique_ptr<double[]> dataBuffer;
    bool connected = false;
};

#endif
