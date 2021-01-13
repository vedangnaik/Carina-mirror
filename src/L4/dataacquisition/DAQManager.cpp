#include "DAQManager.h"

DAQManager::DAQManager(std::vector<AbstractDAQDeviceHandler*> DAQDevices) : DAQDevices{DAQDevices} {
    // Create timer to start reading from DAQs here
    this->DAQReadTimer = new QTimer(this);
    this->DAQReadTimer->start(1000);
}

void DAQManager::startAcquisition() {
    if (sensorToDAQMap.empty()) {
        sensorToDAQMap = SensorToDAQLinker::getSensorToDAQLinks(svgic->getSensorIDs(), this->DAQDevices);
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
    // figure out here how to correclty hook up the channels from all DAQs into ids for defined sensors
    // output random values for now
    if (this->svgic == nullptr) { return; }
    for (std::string id : this->svgic->getSensorIDs()) {
        this->svgic->updateValue(id, rand() % 1000);
    }
}
