#pragma once

#include "SensorValuesGateway.h"
#include "AbstractDAQ.h"
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
    DAQManager(std::vector<AbstractDAQ*> DAQDevices);
    void startAcquisition();
    void getLatestData();
    void stopAcquisition();
    void relinkSensors();
    void setOutputContract(SVGIC* svgic);
private:
    std::vector<AbstractDAQ*> DAQDevices;
    SVGIC* svgic = nullptr;
    QTimer* DAQReadTimer;
    std::map<std::string, std::pair<AbstractDAQ*, unsigned int>> sensorToDAQMap;
    std::mutex sensorLinksMutex;
};
