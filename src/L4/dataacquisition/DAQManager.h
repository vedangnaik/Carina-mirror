#pragma once

#include "SensorValuesGateway.h"
#include "IDAQDeviceHandler.h"
#include "easylogging++.h"
#include <map>
#include <vector>
#include <QObject>
#include <iostream>
#include <QTimer>


class DAQManager : public QObject {
    Q_OBJECT
public:
    DAQManager(std::vector<IDAQDeviceHandler*> DAQDevices);
    void startAcquisition();
    void stopAcquisition();
    void getLatestData();
    void setOutputContract(SVGIC* svgic) {
        this->svgic = svgic;
    }
private:
    std::vector<IDAQDeviceHandler*> DAQDevices;
    SVGIC* svgic = nullptr;
    QTimer* DAQReadTimer;
    // The linker will populate this correctly.
    const std::map<std::string, std::pair<IDAQDeviceHandler*, unsigned int>> sensorIDToDAQMap;
};
