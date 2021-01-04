#pragma once

#ifdef ULDAQ_AVAILABLE
    #include <uldaq.h>
    #include "AiDAQHandler.h"
#endif

#include "SensorValuesGateway.h"
#include "DAQDeviceHandler.h"
#include "easylogging++.h"
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
#ifdef ULDAQ_AVAILABLE
    const DaqDeviceInterface DAQDeviceInterfaceType = ANY_IFC;
#endif
};
