#include "usecases.h"

void SensorManager::addSensors(std::vector<Sensor *> sensors) {
    for (Sensor* s : sensors) {
        this->sensors[s->id] = s;
    }
}
