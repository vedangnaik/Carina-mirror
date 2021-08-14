#include "ActuatorsController.h"

ActuatorsController::ActuatorsController(AMIC& amic) 
    : amic{amic}
{}

void ActuatorsController::setState(std::string id, bool status) {
    this->amic.setState(id, status);
}
