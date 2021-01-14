#ifdef ULDAQ_AVAILABLE
#pragma once

#include "AbstractDAQDeviceHandler.h"
#include "easylogging++.h"
#include <uldaq.h>
#include <stdlib.h>
#include <map>
#include <vector>
#include <memory>
#include <iostream>
#include <cmath>

/**
 * @brief The AiDAQHandler class handles an analog input DAQ device
 */
class AiDAQHandler : public AbstractDAQDeviceHandler {
public:
    AiDAQHandler(std::string id, DaqDeviceHandle handle, unsigned int numChannels, Range voltageRange);
    ~AiDAQHandler();
    void startAcquisition() override;
    std::vector<double> getLatestData() override;
    void stopAcquisition() override;
private:
    // config vars
    const DaqDeviceHandle handle;
    const unsigned int samplesPerChannel = 100;
    const Range voltageRange;
    double rate = 100; // must be non-const for uldaq.h :(
    const AiInputMode aiim = AI_SINGLE_ENDED;
    const ScanOption so = SO_CONTINUOUS;
    const AInScanFlag aisf = AINSCAN_FF_DEFAULT;
    // status vars
    std::unique_ptr<double[]> dataBuffer;
};

#endif
