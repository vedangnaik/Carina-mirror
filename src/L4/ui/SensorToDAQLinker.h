#pragma once

#include <QDialog>
#include <QComboBox>
#include <QLabel>
#include "AbstractDAQ.h"

namespace Ui {
    class SensorToDAQLinker;
}

class SensorToDAQLinker : public QDialog {
    Q_OBJECT
public:
    static std::map<std::string, std::pair<AbstractDAQ*, unsigned int>>
        getSensorToDAQLinks(std::vector<std::string> sensorIDs, std::vector<AbstractDAQ*> DAQDevices);
private:
    explicit SensorToDAQLinker(std::vector<std::string> sensorIDs, std::vector<AbstractDAQ*> DAQDevices, QWidget *parent = nullptr);
    ~SensorToDAQLinker();
    std::map<std::string, std::pair<AbstractDAQ*, unsigned int>> sensorToDAQLinks;
    Ui::SensorToDAQLinker *ui;
    std::vector<AbstractDAQ*> DAQDevices;
private slots:
    void accept() override;
};

