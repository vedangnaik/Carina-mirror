#include "usecases.h"

void SensorsManager::addSensors(std::vector<Sensor *> sensors) {
    for (Sensor* s : sensors) {
        this->sensors[s->id] = s;
    }
}
