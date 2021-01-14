#pragma once

#include "SensorValuesGateway.h"
#include "AbstractDAQDeviceHandler.h"
#include "easylogging++.h"
#include "SensorToDAQLinker.h"
#include <map>
#include <vector>
#include <mutex>
#include <QObject>
#include <iostream>
#include <QTimer>


class DAQManager : public QObject {
    Q_OBJECT
public:
    DAQManager(std::vector<AbstractDAQDeviceHandler*> DAQDevices);
    void startAcquisition();
    void stopAcquisition();
    void getLatestData();
    void relinkSensors();
    void setOutputContract(SVGIC* svgic) {
        this->svgic = svgic;
    }
private:
    std::vector<AbstractDAQDeviceHandler*> DAQDevices;
    SVGIC* svgic = nullptr;
    QTimer* DAQReadTimer;
    std::map<std::string, std::pair<AbstractDAQDeviceHandler*, unsigned int>> sensorToDAQMap;
    std::mutex sensorLinksMutex;
};
