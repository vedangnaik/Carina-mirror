#ifndef DAQDEVICEHANDLER_H
#define DAQDEVICEHANDLER_H

#include <map>
#include <vector>

class DAQDeviceHandler {
public:
    virtual void startAcquisition() = 0;
    virtual void stopAcquisition() = 0;
    virtual std::map<unsigned int, std::vector<double>> getLatestData() = 0;
    virtual ~DAQDeviceHandler() {};
};

#endif // DAQDEVICEHANDLER_H
