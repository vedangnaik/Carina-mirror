#include "DAQManager.h"

DAQManager::DAQManager(std::vector<AbstractDAQ*> DAQDevices, std::map<std::string, std::pair<AbstractDAQ*, unsigned int>> sensorToDAQLinks)
    : DAQDevices{DAQDevices}, sensorToDAQLinks{sensorToDAQLinks}, svgic{nullptr}
{
    this->DAQReadTimer = new QTimer(this);
    this->DAQReadTimer->start(1000);
}

void DAQManager::startAcquisition() {
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
        if (this->svgic == nullptr) {
            LOG(INFO) << p.first << ", channel " << channel << ": " << daq->getLatestData().at(channel);
        } else {
            this->svgic->updateValue(p.first, daq->getLatestData().at(channel));
        }
    }
}
