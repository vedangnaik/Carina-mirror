#ifndef AIDAQHANDLER_H
#define AIDAQHANDLER_H

#include <uldaq.h>
#include <stdlib.h>
#include <map>
#include <vector>

/**
 * @brief The DAQHandler class handles an analog input DAQ
 */
class AiDAQHandler {
public:
    AiDAQHandler(DaqDeviceHandle handle, unsigned int numChannels, Range gain);
    ~AiDAQHandler();
private:
    // event handler callback
//    void dataHandler(DaqDeviceHandle handle, DaqEventType event, unsigned long long eventData, void* anythingElse);
    // config vars
    const DaqDeviceHandle handle;
    const unsigned int samplesPerChannel = 100;
    const unsigned int numChannels;
    const Range gain;
    const double rate = 100;
    const AiInputMode im = AI_SINGLE_ENDED;
    const ScanOption so = SO_DEFAULTIO;
    const AInScanFlag aisf = AINSCAN_FF_DEFAULT;
//    const DaqEventType events = DE_ON_DATA_AVAILABLE;
//    const unsigned long long numSamplesToNotifyFor = 100;
    // status vars
    double* dataBuffer;
    std::map<unsigned int, std::vector<double>> calibratedValues;
    bool connected = false;
};

#endif // AIDAQHANDLER_H
