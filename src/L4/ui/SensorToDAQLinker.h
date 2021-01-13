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
    explicit SensorToDAQLinker(std::vector<std::string> sensorIDs, std::vector<AbstractDAQDeviceHandler*> DAQDevices, QWidget *parent = nullptr);
    ~SensorToDAQLinker();
    static std::map<std::string, std::pair<AbstractDAQDeviceHandler*, unsigned int>> getSensorToDAQLinks(std::vector<std::string> sensorIDs, std::vector<AbstractDAQDeviceHandler*> DAQDevices);
private slots:
    void reject() override;
    void accept() override;
private:
    Ui::SensorToDAQLinker *ui;
};

