#include "DAQManager.h"

DAQManager::DAQManager(std::vector<AbstractDAQ*> DAQDevices, std::map<std::string, std::pair<AbstractDAQ*, unsigned int>> sensorToDAQLinks)
    : DAQDevices{DAQDevices}, sensorToDAQLinks{sensorToDAQLinks}
{
    this->DAQReadTimer = new QTimer(this);
    this->DAQReadTimer->start(1000);
}

void DAQManager::startAcquisition() {
    if (this->svgic == nullptr) {
        LOG(FATAL) << "DAQManager: this->svgic is nullptr. Aborting...";
    }
    for (const auto& d : this->DAQDevices) {
        d->startAcquisition();
    }
    connect(this->DAQReadTimer, &QTimer::timeout, this, &DAQManager::getLatestData);
}

void DAQManager::stopAcquisition() {
    disconnect(this->DAQReadTimer, &QTimer::timeout, this, &DAQManager::getLatestData);
    for (const auto& d : this->DAQDevices) {
        d->stopAcquisition();
    }
}

void DAQManager::getLatestData() {
    for (const auto& p : this->sensorToDAQLinks) {
        const auto& daq = p.second.first;
        const auto& channel = p.second.second;
        this->svgic->updateValue(p.first, daq->getLatestData().at(channel));
    }
}

void DAQManager::setOutputContract(SVGIC* svgic) {
    this->svgic = svgic;
}
