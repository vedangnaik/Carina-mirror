#include "SensorsManager.h"

void SensorsManager::addSensors(std::map<std::string, Sensor*> sensors) {
    this->sensors = sensors;
}

float SensorsManager::getSensorValue(std::string id) {
    auto v = this->sensors[id]->values;
    if (v.size() > 0) {
        return v[v.size() - 1];
    } else {
        return __builtin_nanf64("");
    }
}

void SensorsManager::setSensorValue(std::string id, float value) {
    this->sensors[id]->values.push_back(value);
}

std::vector<std::string> SensorsManager::getSensorIDs() {
    std::vector<std::string> sensorIds;
    for (auto p : this->sensors) {
        sensorIds.push_back(p.first);
    }
    return sensorIds;
}
