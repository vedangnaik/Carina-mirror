#include "SensorsManager.h"

float SensorsManager::getSensorValue(std::string id) {
    return this->sensors.at(id)->values.back();
}

void SensorsManager::setSensorValue(std::string id, float value) {
    this->sensors.at(id)->values.push_back(value);
    this->smoc->displaySensorValue(id, value);
}

std::vector<std::string> SensorsManager::getSensorIDs() {
    std::vector<std::string> sensorIds;
    for (auto p : this->sensors) {
        sensorIds.push_back(p.first);
    }
    return sensorIds;
}
