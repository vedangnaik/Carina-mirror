#pragma once

#ifdef WIRINGPI_AVAILABLE

#include "Actuator.h"
#include "easylogging++.h"
#include <wiringPi.h>

class SolenoidActuator : public Actuator {
public:
    SolenoidActuator(std::string deviceID, uint8_t relayChannel, uint8_t gpioPin, bool nominallyPowered);
    void setState(bool state) override;
private:
    void setDigitalPin(uint8_t pin, bool turnRelayChannelOn);
    const uint8_t gpioPin;
    const uint8_t relayChannel;
    const bool nominallyPowered;
};

#endif