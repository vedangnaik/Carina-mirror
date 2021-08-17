#include "SensorsManager.h"

SensorsManager::SensorsManager(std::unordered_map<std::basic_string<char>, std::unique_ptr<Sensor>>& sensors, SMOC& smoc)
    : sensors{std::move(sensors)}, smoc{smoc}
{
    this->DAQReadTimer = new QTimer(this);
    this->DAQReadTimer->start(1);
    this->UIUpdateTimer = new QTimer(this);
    this->UIUpdateTimer->start(30);
}

void SensorsManager::startAcquisition() {
    for (const auto& p : this->sensors) {
        const std::unique_ptr<Sensor>& s = p.second;
        s->startAcquisition();
    }
    connect(this->DAQReadTimer, &QTimer::timeout, this, &SensorsManager::getLatestData);
    connect(this->UIUpdateTimer, &QTimer::timeout, this, &SensorsManager::updateUI);
}

void SensorsManager::stopAcquisition() {
    disconnect(this->DAQReadTimer, &QTimer::timeout, this, &SensorsManager::getLatestData);
    disconnect(this->UIUpdateTimer, &QTimer::timeout, this, &SensorsManager::updateUI);
    for (const auto& p : this->sensors) {
        const std::unique_ptr<Sensor>& s = p.second;
        s->stopAcquisition();
    }
}

double SensorsManager::getSensorValue(std::string id) {
    try {
        // return this->sensors.at(id).values.back();
        // Make this go through the vector and give the correct latestValue
        return 0.0;
    }  catch (std::out_of_range& e) {
        // Precondition violation, it's over.
        LOG(FATAL) << "SensorsManager::getSensorValue(" << id << "): ID not found. Exception: " << e.what();
        std::terminate(); // just to stop the compiler complaining about no return value xD
    }
}

void SensorsManager::getLatestData() {
    this->valuesToDisplay.clear();
    for (const auto& p : this->sensors) {
        const std::unique_ptr<Sensor>& s = p.second;
        const double value = s->getLatestData();
        CLOG(INFO, "sensorValueLogger") << s->id << "," << value;
        this->valuesToDisplay[s->id] = value;
    }
}

void SensorsManager::updateUI() {
    for (const auto& p : this->valuesToDisplay) {
        this->smoc.notify(p.first, p.second);
    }
}