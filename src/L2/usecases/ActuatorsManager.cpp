#include "ActuatorsManager.h"

ActuatorsManager::ActuatorsManager(std::vector<Actuator*> actuators, AMOC& amoc)
    : actuators{std::move(actuators)}, amoc{amoc}
{}

bool
ActuatorsManager::getActuatorStatus(std::string id)
{
    for (const auto& a : this->actuators) {
        if (a->id == id) {
            return a->state;
        }
    }

    LOG(FATAL) << "ActuatorsManager::getActuatorStatus(" << id << "): ID not found. Terminating...";
    std::terminate();
}

void
ActuatorsManager::setState(std::string id, bool status)
{
    for (const auto& a : this->actuators) {
        if (a->id == id) {
            a->setState(status);
            this->amoc.notify(id, status);
            return;
        }
    }

    LOG(FATAL) << "ActuatorsManager::getActuatorStatus(" << id << "): ID not found. Terminating...";
    std::terminate();
}
