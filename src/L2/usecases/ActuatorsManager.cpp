#include "ActuatorsManager.h"

ActuatorsManager::ActuatorsManager(map<const string, Actuator> actuators, AMOC& amoc)
    : actuators{std::move(actuators)}, amoc{amoc}
{}

bool
ActuatorsManager::getActuatorStatus(string id)
{
    try {
        return this->actuators.at(id).status;
    } catch (std::out_of_range& e) {
        // Precondition violation, it's over.
        LOG(FATAL) << "ActuatorsManager::getActuatorStatus(" << id << "): ID not found. Exception: " << e.what();  
        std::terminate();
    }
}

void
ActuatorsManager::actuate(string id)
{
    // Make sure this actuator is present.
    try {
        bool currentState = this->actuators.at(id).status;
        this->actuators.at(id).status = !currentState;
        this->amoc.notify(id, this->actuators.at(id).status);
    } catch (std::out_of_range& e) {
        // Precondition violation, it's over.
        LOG(FATAL) << "ActuatorsManager::getActuatorStatus(" << id << "): ID not found. Exception: " << e.what();
    }
}
