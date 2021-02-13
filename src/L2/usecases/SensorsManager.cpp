#include "SensorsManager.h"

SensorsManager::SensorsManager(map<const string, Sensor> sensors, SMOC& smoc)
    : sensors{std::move(sensors)}, smoc{smoc}
{}

float SensorsManager::getSensorValue(string id) {
    try {
        return this->sensors.at(id).values.back();
    }  catch (std::out_of_range& e) {
        // Precondition violation, it's over.
        LOG(FATAL) << "SensorsManager::getSensorValue(" << id << "): ID not found. Exception: " << e.what();
        std::terminate(); // just to stop the compiler complaining about no return value xD
    }
}

void SensorsManager::setSensorValue(string id, float value) {
    try {
        this->sensors.at(id).values.push_back(value);
        this->smoc.notify(id, value);
    }  catch (std::out_of_range& e) {
        // Precondition violation, it's over.
        LOG(FATAL) << "SensorsManager::setSensorValue(" << id << ", " << value << "): ID not found. Exception: " << e.what();
        std::terminate(); // just to stop the compiler complaining about no return value xD
    }
}

std::vector<string> SensorsManager::getSensorIDs() {
    std::vector<string> sensorIDs;
    for (const auto& p : sensors) {
        sensorIDs.push_back(p.first);
    }
    return sensorIDs;
}
