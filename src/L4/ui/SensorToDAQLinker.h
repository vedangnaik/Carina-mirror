#pragma once

#include <QDialog>
#include <QComboBox>
#include <QLabel>
#include "AbstractDAQDeviceHandler.h"

namespace Ui {
    class SensorToDAQLinker;
}

class SensorToDAQLinker : public QDialog {
    Q_OBJECT
public:
    static std::map<std::string, std::pair<AbstractDAQDeviceHandler*, unsigned int>>
        getSensorToDAQLinks(std::vector<std::string> sensorIDs, std::vector<AbstractDAQDeviceHandler*> DAQDevices);
private:
    explicit SensorToDAQLinker(std::vector<std::string> sensorIDs, std::vector<AbstractDAQDeviceHandler*> DAQDevices, QWidget *parent = nullptr);
    ~SensorToDAQLinker();
    void accept() override;
    std::map<std::string, std::pair<AbstractDAQDeviceHandler*, unsigned int>> sensorToDAQLinks;
    Ui::SensorToDAQLinker *ui;
    std::vector<AbstractDAQDeviceHandler*> DAQDevices;
};

