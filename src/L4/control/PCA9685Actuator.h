#pragma once

#ifdef WIRINGPI_AVAILABLE

#include "Actuator.h"
#include "Adafruit_PWMServoDriver.h"
#include <cstdint>
#include <cmath>
#include "easylogging++.h"
#include <cassert>

class PCA9685Actuator : public Actuator {
public:
    // constructor to set custom closed and open angle
    PCA9685Actuator(std::string id, uint8_t channel, double openAngle, double closedAngle, uint16_t SERVOMIN, uint16_t SERVMOMAX, double ANGLEMIN, double ANGLEMAX, std::shared_ptr<Adafruit_PWMServoDriver> pwm);
    void setState(bool state) override;
private:
    void rotateToAngle(double angle);
    uint16_t getPWMFromAngle(double angle) const;
    uint8_t channel;
    double openAngle;
    double closedAngle;
    uint16_t SERVOMIN;
    uint16_t SERVOMAX;
    double ANGLEMIN;
    double ANGLEMAX;
    std::shared_ptr<Adafruit_PWMServoDriver> pwm;
};

#endif