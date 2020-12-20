#ifndef DAQMANAGER_H
#define DAQMANAGER_H

#include "src/L3/gateways/SensorValuesGateway.h"
#include "src/L4/dataacquisition/AiDAQHandler.h"
#include <map>
#include <vector>
#include <QObject>
#include <uldaq.h>
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
    // device search parameters
    const DaqDeviceInterface DAQDeviceInterfaceType = ANY_IFC;
};

#endif // DAQMANAGER_H
