#include "Actuator.h"

Actuator::Actuator(std::string id, std::string name) {
    this->id = id;
    this->name = name;
    this->status = false;
}
