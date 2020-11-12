#include "ActuatorController.h"

ActuatorsController::ActuatorsController(AMIC* amic) {
    this->amic = amic;
}

void ActuatorsController::actuate(std::string id) {
    this->amic->actuate(id);
}
