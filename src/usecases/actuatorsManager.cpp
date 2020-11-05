#include "usecases.h"

void ActuatorsManager::addActuators(std::vector<Actuator *> actuators) {
    for (Actuator* a : actuators) {
        this->actuators[a->id] = a;
    }
}
