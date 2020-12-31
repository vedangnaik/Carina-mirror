#include "SensorsManager.h"

SensorsManager::SensorsManager(const std::map<std::string, Sensor*> sensors) : sensors(sensors) {
    for (const auto& [id, sensor] : sensors) {
        if (sensor == nullptr) {
            throw NullptrSensorError(id);
        }
    }
}

float SensorsManager::getSensorValue(std::string id) {
    try {
        return this->sensors.at(id)->values.back();
    }  catch (std::out_of_range& e) {
        // Precondition violation, it's over.
        LOG(FATAL) << "SensorsManager::getSensorValue(" << id << "): ID not found. Exception: " << e.what();
        std::terminate();
    }
}

void SensorsManager::setSensorValue(std::string id, float value) {
    try {
        this->sensors.at(id)->values.push_back(value);
        this->smoc->notify(id, value);
    }  catch (std::out_of_range& e) {
        // Precondition violation, it's over.
        LOG(FATAL) << "SensorsManager::setSensorValue(" << id << ", " << value << "): ID not found. Exception: " << e.what();
        std::terminate();
    }
}

std::vector<std::string> SensorsManager::getSensorIDs() {
    std::vector<std::string> sensorIds;
    for (const auto& [id, _] : this->sensors) {
        sensorIds.push_back(id);
    }
    return sensorIds;
}
