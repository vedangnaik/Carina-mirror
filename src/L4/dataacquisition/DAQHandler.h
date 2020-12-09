#ifndef MCCDAQ_H
#define MCCDAQ_H

#include <uldaq.h>

class DAQHandler {
public:
    DAQHandler(DaqDeviceHandle handle);
private:
    const DaqDeviceHandle handle;
    const int samplesPerChan = 1000;
};

#endif // MCCDAQ_H
