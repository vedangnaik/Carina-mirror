#include "DAQManager.h"

DAQManager::DAQManager(std::vector<AbstractDAQ*> DAQDevices, SVGIC& svgic)
    : DAQDevices{DAQDevices}, sensorToDAQLinks{}, svgic{svgic}
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
        const double value = daq->getLatestData().at(channel);
        CLOG(INFO, "sensorValueLogger") << daq->deviceID << "-" << channel << "," << value;
        this->svgic.updateValue(p.first, value);
    }
}
