#include "ActuatorsController.h"

ActuatorsController::ActuatorsController(AMIC* amic) {
    this->amic = amic;
}

void ActuatorsController::actuate(std::string id) {
    this->amic->actuate(id);
    ActuatorStatusGateway* asg = new ActuatorStatusGateway("actuatorStatuses.json");
    asg->updateActutatorStatus(id, this->amic->getActuatorStatus(id));
}
