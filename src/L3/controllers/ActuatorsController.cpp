#include "ActuatorsController.h"

void ActuatorsController::actuate(std::string id) {
    this->amic.actuate(id);
}
