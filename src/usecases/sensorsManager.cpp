#include "usecases.h"

void SensorsManager::addSensors(std::vector<Sensor *> sensors) {
    for (Sensor* s : sensors) {
        this->sensors[s->id] = s;
    }
}

Sensor* SensorsManager::findSensor(std::string id) {
    try {
        return this->sensors[id];
    }  catch (std::out_of_range e) {
        return nullptr;
    }
}
