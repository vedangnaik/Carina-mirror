#include "ActuatorsManager.h"

ActuatorsManager::ActuatorsManager(std::map<std::string, Actuator*> actuators) : actuators(actuators) {
    for (const auto& [id, actuator] : this->actuators) {
        if (actuator == nullptr) {
            throw NullptrActuatorError(id);
        }
    }
}

bool ActuatorsManager::getActuatorStatus(std::string id) {
    try {
        return this->actuators.at(id)->status;
    } catch (std::out_of_range& e) {
        // Precondition violation, it's over.
        LOG(FATAL) << "ActuatorsManager::getActuatorStatus(" << id << "): ID not found. Exception: " << e.what();
        std::terminate();
    }
}

void ActuatorsManager::actuate(std::string id) {
    Actuator* a;

    // Make sure this actuator is present.
    try {
        a  = this->actuators.at(id);
        a->status = !a->status;
    } catch (std::out_of_range& e) {
        // Precondition violation, it's over.
        LOG(FATAL) << "ActuatorsManager::getActuatorStatus(" << id << "): ID not found. Exception: " << e.what();
        std::terminate();
    }

    // Make sure that this display output is not null.
    if (this->amoc == nullptr) {
        LOG(ERROR) << "ActuatorsManager::actuate(" << id << "): this->amoc* is nullptr, not displaying actuator state.";
    } else {
        this->amoc->notify(id, a->status);
    }
}
