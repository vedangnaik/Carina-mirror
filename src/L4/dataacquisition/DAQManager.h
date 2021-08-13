#pragma once

#include "SensorValuesGateway.h"
#include "AbstractDAQ.h"
#include "easylogging++.h"
#include <map>
#include <vector>
#include <QObject>
#include <iostream>
#include <QTimer>

class DAQManager : public QObject {
    Q_OBJECT
public:
    DAQManager(std::vector<Sensor*> DAQDevices, SVGIC& svgic);
    void startAcquisition();
    void getLatestData();
    void stopAcquisition();
    std::vector<std::string> getSensorIDs();

    const std::vector<Sensor*> DAQDevices;
    std::map<std::string, std::pair<Sensor*, unsigned int>> sensorToDAQLinks;
private:
    void updateUI();

    SVGIC& svgic;
    QTimer* DAQReadTimer;
    QTimer* UIUpdateTimer;
    std::map<std::string, double> valuesToDisplay;
};
