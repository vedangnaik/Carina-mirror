#include "usecases.h"

void ActuatorsManager::addActuators(std::vector<Actuator *> actuators) {
    for (Actuator* a : actuators) {
        this->actuators[a->id] = a;
    }
}

Actuator* ActuatorsManager::findActuator(std::string id) {
    try {
        return this->actuators.at(id);
    }  catch (std::out_of_range e) {
        return nullptr;
    }
}
