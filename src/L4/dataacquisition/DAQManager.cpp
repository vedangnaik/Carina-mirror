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
    for (const auto& p : this->sensorToDAQMap) {
        const auto& daq = p.second.first;
        const auto& channel = p.second.second;
        this->svgic->updateValue(p.first, daq->getLatestData().at(channel));
    }
}

void DAQManager::relinkSensors() {
    std::lock_guard<std::mutex> guard(this->sensorLinksMutex);
    this->sensorToDAQMap = SensorToDAQLinker::getSensorToDAQLinks(svgic->getSensorIDs(), this->DAQDevices);
}

void DAQManager::setOutputContract(SVGIC* svgic) {
    this->svgic = svgic;
}
