#include "DummyActuator.h"

DummyActuator::DummyActuator(const std::string& id)
    : Actuator(id)
{}

void DummyActuator::setState(const bool state) {
    this->state = state;
    LOG(INFO) << "DummyActuator id '" << this->id << "' is now in state '" << state << "'.";
}