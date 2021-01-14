#include "DAQManager.h"

DAQManager::DAQManager(std::vector<AbstractDAQDeviceHandler*> DAQDevices) : DAQDevices{DAQDevices} {
    this->DAQReadTimer = new QTimer(this);
    this->DAQReadTimer->start(1000);
}

void DAQManager::startAcquisition() {
    if (sensorToDAQMap.empty()) { this->relinkSensors(); }
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
    if (this->svgic == nullptr) { return; }
    for (std::string id : this->svgic->getSensorIDs()) {
        const auto& daq = this->sensorToDAQMap.at(id).first;
        const auto& channel = this->sensorToDAQMap.at(id).second;
        this->svgic->updateValue(id, daq->getLatestData().at(channel));
    }
}

void DAQManager::relinkSensors() {
    std::lock_guard<std::mutex> guard(this->sensorLinksMutex);
    this->sensorToDAQMap = SensorToDAQLinker::getSensorToDAQLinks(svgic->getSensorIDs(), this->DAQDevices);
}
