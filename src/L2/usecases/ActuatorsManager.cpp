#include "ActuatorsManager.h"

ActuatorsManager::ActuatorsManager(std::map<std::string, Actuator*> actuators) : actuators(actuators) {

}

bool ActuatorsManager::getActuatorStatus(std::string id) {
    return this->actuators.at(id)->status;
}

void ActuatorsManager::actuate(std::string id) {
    try {
        Actuator* a  = this->actuators.at(id);
        a->status = !a->status;
        this->amoc->notify(id, a->status);
    }  catch (std::out_of_range& e) {
        // shit
    }
}
