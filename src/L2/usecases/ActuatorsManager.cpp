#include "ActuatorsManager.h"

ActuatorsManager::ActuatorsManager(std::unordered_map<std::string, std::unique_ptr<Actuator>>& actuators, AMOC& amoc)
    : actuators{std::move(actuators)}, amoc{amoc}
{}

bool
ActuatorsManager::getActuatorStatus(std::string id)
{
    for (const auto& p : this->actuators) {
        const std::unique_ptr<Actuator>& a = p.second;
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
    for (const auto& p : this->actuators) {
        const std::unique_ptr<Actuator>& a = p.second;
        if (a->id == id) {
            a->setState(status);
            this->amoc.notify(id, status);
            return;
        }
    }

    LOG(FATAL) << "ActuatorsManager::getActuatorStatus(" << id << "): ID not found. Terminating...";
    std::terminate();
}
