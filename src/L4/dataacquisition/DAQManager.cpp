#include "DAQManager.h"

DAQManager::DAQManager(std::vector<AbstractDAQ*> DAQDevices, SVGIC& svgic)
    : DAQDevices{DAQDevices}, sensorToDAQLinks{}, svgic{svgic}
{
    this->DAQReadTimer = new QTimer(this);
    this->DAQReadTimer->start(1);
    this->UIUpdateTimer = new QTimer(this);
    this->UIUpdateTimer->start(30);
}

std::vector<std::string> DAQManager::getSensorIDs() {
    return this->svgic.getSensorIDs();
}

void DAQManager::startAcquisition() {
    for (const auto& d : this->DAQDevices) {
        d->startAcquisition();
    }
    connect(this->DAQReadTimer, &QTimer::timeout, this, &DAQManager::getLatestData);
    connect(this->UIUpdateTimer, &QTimer::timeout, this, &DAQManager::updateUI);
}

void DAQManager::stopAcquisition() {
    disconnect(this->DAQReadTimer, &QTimer::timeout, this, &DAQManager::getLatestData);
    disconnect(this->UIUpdateTimer, &QTimer::timeout, this, &DAQManager::updateUI);
    for (const auto& d : this->DAQDevices) {
        d->stopAcquisition();
    }
}

void DAQManager::getLatestData() {
    this->valuesToDisplay.clear();
    for (const auto& p : this->sensorToDAQLinks) {
        const auto& daq = p.second.first;
        const auto& channel = p.second.second;
        const double value = daq->getLatestData().at(channel);
//        this->tempfile << std::chrono::duration_cast<std::chrono::milliseconds>(
//                              std::chrono::system_clock::now().time_since_epoch()
//                          ).count() << " " << daq->deviceID << "-" << channel << "," << value << '\n';
        CLOG(INFO, "sensorValueLogger") << daq->deviceID << "-" << channel << "," << value;
        this->valuesToDisplay[p.first] = value;
    }
}

void DAQManager::updateUI() {
    for (const auto& p : this->valuesToDisplay) {
        this->svgic.updateValue(p.first, p.second);
    }
}
