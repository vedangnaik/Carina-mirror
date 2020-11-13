#include "ActuatorsManager.h"

void ActuatorsManager::addActuators(std::map<std::string, Actuator*> actuators) {
    this->actuators = actuators;
}

bool ActuatorsManager::getActuatorStatus(std::string id) {
    return this->actuators[id]->status;
}

void ActuatorsManager::actuate(std::string id) {
    try {
        Actuator* a  = this->actuators.at(id);
        a->status = !a->status;
    }  catch (std::out_of_range& e) {
        // shit
    }
}
