#include "SensorsManager.h"

void SensorsManager::addSensors(std::map<std::string, Sensor*> sensors) {
    this->sensors = sensors;
}

float SensorsManager::getSensorValue(std::string id) {
    auto v = this->sensors[id]->values;
    return v[v.size() - 1];
}

void SensorsManager::setSensorValue(std::string id, float value) {
    this->sensors[id]->values.push_back(value);
}

std::vector<std::string> SensorsManager::getSensorIDs() {
    std::vector<std::string> sensorIds(this->sensors.size());
    for (auto p : this->sensors) {
        sensorIds.push_back(p.first);
    }
    return sensorIds;
}
