#ifndef DAQMANAGER_H
#define DAQMANAGER_H

#include <map>
#include <vector>
#include <QObject>
#include "src/L2/services/ClocksModule.h"
#include "src/L3/gateways/SensorValuesGateway.h"
#include <uldaq.h>
#include "AiDAQHandler.h"

class DAQManager : public QObject {
    Q_OBJECT
public:
    DAQManager(ClocksModule& cm, SVGIC& svgic);
    void startAcquisition();
    void stopAcquisition();
    void getLatestData();
private:
    std::vector<DAQDeviceHandler*> DAQDevices;
    ClocksModule& cm;
    SVGIC& svgic;
    // device search parameters
    const unsigned int maxDAQDevices = 100;
    const DaqDeviceInterface DAQDeviceInterfaceType = ANY_IFC;
};

#endif // DAQMANAGER_H
