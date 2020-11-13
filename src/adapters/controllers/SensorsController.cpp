#include "SensorsController.h"

SensorsController::SensorsController(SMIC* smic) {
    this->smic = smic;
}

void SensorsController::updateValue(std::string id, float value) {
    this->smic->setSensorValue(id, value);
}
