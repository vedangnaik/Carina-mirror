#pragma once

#ifdef WIRINGPI_AVAILABLE

#include "Actuator.h"
#include "easylogging++.h"

class SolenoidActuator : public Actuator {
public:
    SolenoidActuator(std::string id, uint8_t relayChannel, uint8_t gpioPin, bool nominallyPowered);
    void setState(bool state) override;
private:
    void setDigitalPin(uint8_t pin, bool turnRelayChannelOn);
    uint8_t gpioPin;
    bool nominallyPowered;
};

#endif