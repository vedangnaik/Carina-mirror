#pragma once

#ifdef WIRINGPI_AVAILABLE

#include "Actuator.h"
#include "Adafruit_PWMServoDriver.h"
#include <cstdint>
#include <cmath>
#include "easylogging++.h"
#include <cassert>
#include <type_traits>

class PCA9685Actuator : public Actuator {
public:
    PCA9685Actuator(std::string id, uint8_t channel, double openAngle, double closedAngle, double overdriveAngle, int overdriveDelay, uint16_t SERVOMIN, uint16_t SERVMOMAX, double ANGLEMIN, double ANGLEMAX, std::shared_ptr<Adafruit_PWMServoDriver> pwm);
    void setState(bool state) override;
private:
    void rotateOverdrive(double angle, double overAngle);
    void rotateToAngle(double angle);
    uint16_t getPWMFromAngle(double angle) const;
    uint8_t channel;
    double openAngle;
    double closedAngle;
    double overdriveAngle;
    int overdriveDelay;
    uint16_t SERVOMIN;
    uint16_t SERVOMAX;
    double ANGLEMIN;
    double ANGLEMAX;
    std::shared_ptr<Adafruit_PWMServoDriver> pwm;
};

#endif
