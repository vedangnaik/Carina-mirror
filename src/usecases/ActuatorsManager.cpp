#include "ActuatorsManager.h"

void ActuatorsManager::addActuators(std::vector<Actuator *> actuators) {
    for (Actuator* a : actuators) {
        this->actuators[a->id] = a;
    }
}

Actuator* ActuatorsManager::findActuator(std::string id) {
    try {
        return this->actuators.at(id);
    }  catch (std::out_of_range& e) {
        return nullptr;
    }
}

void ActuatorsManager::actuate(std::string id) {
    try {
        Actuator* a  = this->actuators.at(id);
        a->status = !a->status;
    }  catch (std::out_of_range& e) {
        // shit
    }
}
