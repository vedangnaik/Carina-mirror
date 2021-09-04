#ifdef WIRINGPI_AVAILABLE

#include "SolenoidActuator.h"

SolenoidActuator::SolenoidActuator(std::string id, uint8_t pin)
    : Actuator(id)
{}

void DummyActuator::setState(const bool state) {
    //
}

#endif