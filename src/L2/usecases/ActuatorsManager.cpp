#include "ActuatorsManager.h"

ActuatorsManager::ActuatorsManager(map<const string, Actuator> actuators)
    : actuators{std::move(actuators)}
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
    try {
        // Make sure this actuator is present.
        Actuator& a = this->actuators.at(id);
        a.status = !a.status;
        // Make sure that this display output is not null.
        if (this->amoc == nullptr) {
            LOG(ERROR) << "ActuatorsManager::actuate(" << id << "): this->amoc is nullptr, not displaying actuator state.";
        } else {
            this->amoc->notify(id, a.status);
        }
    } catch (std::out_of_range& e) {
        // Precondition violation, it's over.
        LOG(FATAL) << "ActuatorsManager::getActuatorStatus(" << id << "): ID not found. Exception: " << e.what();
        std::terminate();
    }
}
