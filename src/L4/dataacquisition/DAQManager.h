#pragma once

#include "SensorValuesGateway.h"
#include "AbstractDAQ.h"
#include "easylogging++.h"
#include <map>
#include <vector>
#include <mutex>
#include <QObject>
#include <iostream>
#include <QTimer>

class DAQManager : public QObject {
    Q_OBJECT
public:
    DAQManager(std::vector<AbstractDAQ*> DAQDevices, std::map<std::string, std::pair<AbstractDAQ*, unsigned int>> sensorToDAQLinks);
    void startAcquisition();
    void getLatestData();
    void stopAcquisition();
    void setOutputContract(SVGIC* svgic);

    const std::vector<AbstractDAQ*> DAQDevices;
    std::map<std::string, std::pair<AbstractDAQ*, unsigned int>> sensorToDAQLinks;
private:
    SVGIC* svgic;
    QTimer* DAQReadTimer;
    std::mutex sensorLinksMutex;
};
