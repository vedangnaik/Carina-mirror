#include "ActuatorsController.h"

void ActuatorsController::actuate(std::string id) {
    this->amic.actuate(id);
    ActuatorStatusGateway* asg = new ActuatorStatusGateway("actuatorStatuses.json");
    asg->updateActutatorStatus(id, this->amic.getActuatorStatus(id));
}
