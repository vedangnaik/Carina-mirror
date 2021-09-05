#ifdef WIRINGPI_AVAILABLE

#include "SolenoidActuator.h"

SolenoidActuator::SolenoidActuator(std::string deviceID, uint8_t relayChannel, uint8_t gpioPin, bool nominallyPowered)
    : Actuator(std::move(deviceID)), gpioPin{gpioPin}, relayChannel{relayChannel}, nominallyPowered{nominallyPowered}
{}

void SolenoidActuator::setState(const bool state) {
    /* four bools to keep track of:
     * - state on or off
     * - relay connection nominally closed or open
     * - gpio pin high or low (whether we want to turn relay off or on respectively)
     * - solenoid nominally open or closed
    */
    this->state = state;
    if (this->state == nominallyPowered) {
        LOG(INFO) << "Solenoid '" << this->id << "' at relay channel " << relayChannel << " is now turned OFF.";
    } else {
        LOG(INFO) << "Solenoid '" << this->id << "' at relay channel " << relayChannel << " is now turned ON.";
    }

    setDigitalPin(gpioPin, this->state);
}

void SolenoidActuator::setDigitalPin(uint8_t pin, bool turnRelayChannelOn) {
    // https://www.digikey.ca/en/maker/blogs/2019/how-to-use-gpio-on-the-raspberry-pi-with-c
    pinMode(pin, OUTPUT);

    if (turnRelayChannelOn) {
        digitalWrite(pin, 0);  // yes, activating the channel means pulling it to ground
    } else {
        digitalWrite(pin, 1);
    }
}

#endif