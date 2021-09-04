#ifdef WIRINGPI_AVAILABLE

#include "SolenoidActuator.h"

SolenoidActuator::SolenoidActuator(std::string id, uint8_t relayChannel, uint8_t gpioPin, bool nominallyPowered)
    : Actuator(std::move(id)), relayChannel{relayChannel}, gpioPin{gpioPin}, nominallyPowered{nominallyPowered}
{}

void SolenoidActuator::setState(const bool state) {
    this->state = state;
    if (this->state == nominallyPowered) {
        LOG(INFO) << "Solenoid '" << this->id << "' at relay channel " << relayChannel << " is now turned OFF.";
        setDigitalPin(gpioPin, false);
    } else {
        LOG(INFO) << "Solenoid '" << this->id << "' at relay channel " << relayChannel << " is now turned ON.";
        setDigitalPin(gpioPin, true);
    }
}

void SolenoidActuator::setDigitalPin(uint8_t pin, bool turnRelayChannelOn) {
    //
}

#endif