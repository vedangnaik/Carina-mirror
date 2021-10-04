#pragma once

#include "Sensor.h"
#include "easylogging++.h"
#include <string>
#include <map>
#include <vector>
#include <stdexcept>
#include <QObject>
#include <QTimer>

class SMOC {
public:
    virtual void notify(std::string id, double value) = 0;
    virtual ~SMOC() = default;
};

class SMIC {
public:
    virtual double getSensorValue(std::string id) = 0;
    virtual ~SMIC() = default;
private:

};

class SensorsManager : public QObject, public SMIC {
    Q_OBJECT
public:
    SensorsManager(std::unordered_map<std::basic_string<char>, std::unique_ptr<Sensor>>& sensors, SMOC& smoc);
    void startAcquisition();
    void getLatestData();
    void stopAcquisition();
    double getSensorValue(std::string id) override;

private:
    void updateUI();

    std::unordered_map<std::basic_string<char>, std::unique_ptr<Sensor>> sensors;
    SMOC& smoc;
    QTimer* DAQReadTimer;
    QTimer* UIUpdateTimer;
    std::map<std::string, double> valuesToDisplay;
};

