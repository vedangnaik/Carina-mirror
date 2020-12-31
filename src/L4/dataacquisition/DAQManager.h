#ifndef DAQMANAGER_H
#define DAQMANAGER_H

#if defined(__linux__)
    #include <uldaq.h>
    #include "AiDAQHandler.h"
#else
    // include dummy daq here
#endif

#include "SensorValuesGateway.h"
#include "DAQDeviceHandler.h"
#include <map>
#include <vector>
#include <QObject>
#include <iostream>
#include <QTimer>


class DAQManager : public QObject {
    Q_OBJECT
public:
    DAQManager(SVGIC& svgic);
    void startAcquisition();
    void stopAcquisition();
    void getLatestData();
private:
    std::vector<DAQDeviceHandler*> DAQDevices;
    SVGIC& svgic;
    QTimer* DAQReadTimer;
#if defined(__linux__)
    const DaqDeviceInterface DAQDeviceInterfaceType = ANY_IFC;
#endif
};

#endif // DAQMANAGER_H
