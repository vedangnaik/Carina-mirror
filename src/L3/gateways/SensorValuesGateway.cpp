#include "SensorValuesGateway.h"

void SensorValuesGateway::updateValue(std::string id, float value) {
    this->smic.setSensorValue(id, value);
}

std::vector<std::string> SensorValuesGateway::getSensorIDs() {
    return this->smic.getSensorIDs();
}
