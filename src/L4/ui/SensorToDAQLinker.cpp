#include "SensorToDAQLinker.h"
#include "ui_SensorToDAQLinker.h"

SensorToDAQLinker::SensorToDAQLinker(std::vector<std::string> sensorIDs, std::vector<AbstractDAQDeviceHandler*> DAQDevices, QWidget *parent) {
    ui->setupUi(this);

    for (const auto& id : sensorIDs) {
        QComboBox* cmb = new QComboBox(this);
        for (const auto& d : DAQDevices) {
            for (unsigned int i = 0; i < d->numChannels; i++) {
                cmb->addItem(QString::fromStdString(d->deviceID + "-" + std::to_string(i)));
            }
        }
        this->ui->sensorAndDaqFormLayout->addRow(QString::fromStdString(id), cmb);
    }


    connect(ui->closeButton, &QPushButton::clicked, this, &SensorToDAQLinker::reject);
    connect(ui->linkButton, &QPushButton::clicked, this, &SensorToDAQLinker::accept);
}

SensorToDAQLinker::~SensorToDAQLinker() {
    delete ui;
}

std::map<std::string, std::pair<AbstractDAQDeviceHandler*, unsigned int>> SensorToDAQLinker::getSensorToDAQLinks(std::vector<std::string> sensorIDs, std::vector<AbstractDAQDeviceHandler*> DAQDevices) {
    SensorToDAQLinker* stdl = new SensorToDAQLinker(sensorIDs, DAQDevices);
    int r = stdl->exec();
    if (r == QDialog::Accepted) {
        std::map<std::string, std::pair<AbstractDAQDeviceHandler*, unsigned int>> sensorToDAQLinks;
        return sensorToDAQLinks;
    } else {
        return {};
    }
}

void SensorToDAQLinker::accept() {

}

void SensorToDAQLinker::reject() {

}
