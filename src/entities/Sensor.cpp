#include "Sensor.h"

Sensor::Sensor(std::string id, std::string name) {
    this->id = id;
    this->name = name;
    this->values = {};
}
