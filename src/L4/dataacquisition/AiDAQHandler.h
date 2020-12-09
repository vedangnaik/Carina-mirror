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
    void startAcqusition();
    std::map<unsigned int, std::vector<double>> getLatestData();
    void stopAcqution();
private:
    // config vars
    const DaqDeviceHandle handle;
    const unsigned int samplesPerChannel = 100;
    const unsigned int numChannels;
    const Range gain;
    double rate = 100; // must be non-const for uldaq.h :(
    const AiInputMode aiim = AI_SINGLE_ENDED;
    const ScanOption so = SO_DEFAULTIO;
    const AInScanFlag aisf = AINSCAN_FF_DEFAULT;
    // status vars
    double* dataBuffer;
    bool connected = false;
};

#endif // AIDAQHANDLER_H
