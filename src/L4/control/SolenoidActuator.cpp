#ifdef WIRINGPI_AVAILABLE

#include "SolenoidActuator.h"

SolenoidActuator::SolenoidActuator(std::string id, uint8_t relayChannel, uint8_t gpioPin, bool nominallyPowered)
    : Actuator(id)
{}

void SolenoidActuator::setState(const bool state) {
    this->state = state;
    if (this->state == nominallyPowered) {
        LOG(INFO) << "SolenoidActuator id '" << this->id << "' is now turned 'OFF'.";
    } else {
        LOG(INFO) << "SolenoidActuator id '" << this->id << "' is now turned 'ON'.";
    }

    if (this->state) {
        setDigitalPin(gpioPin, false);
    } else {
        setDigitalPin(gpioPin, true);
    }
}

void SolenoidActuator::setDigitalPin(uint8_t pin, bool turnRelayChannelOn) {
    //
}

#endif