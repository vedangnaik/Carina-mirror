#include "DAQManager.h"

DAQManager::DAQManager(std::vector<AbstractDAQ*> DAQDevices) : DAQDevices{DAQDevices} {
    this->DAQReadTimer = new QTimer(this);
    this->DAQReadTimer->start(1000);
}

void DAQManager::startAcquisition() {
    if (this->svgic == nullptr) {
        LOG(FATAL) << "DAQManager: this->svgic is nullptr. Aborting...";
    }
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
    for (std::string id : this->svgic->getSensorIDs()) {
        // Precondition: id and channel will always been in the map, svgic will not be null.
        const auto& daq = this->sensorToDAQMap.at(id).first;
        const auto& channel = this->sensorToDAQMap.at(id).second;
        this->svgic->updateValue(id, daq->getLatestData().at(channel));
    }
}

void DAQManager::relinkSensors() {
    std::lock_guard<std::mutex> guard(this->sensorLinksMutex);
    this->sensorToDAQMap = SensorToDAQLinker::getSensorToDAQLinks(svgic->getSensorIDs(), this->DAQDevices);
}

void DAQManager::setOutputContract(SVGIC* svgic) {
    this->svgic = svgic;
}
