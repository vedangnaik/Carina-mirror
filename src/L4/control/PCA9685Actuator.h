#pragma once

#ifdef WIRINGPI_AVAILABLE

#include "AbstractActuator.h"
#include "Adafruit_PWMServoDriver.h"
#include <cstdint>
#include <cmath>
#include "easylogging++.h"
#include <cassert>

class PCA9685Config {
public:
    PCA9685Config(uint16_t SERVOMIN, uint16_t SERVMOMAX, double ANGLEMIN, double ANGLEMAX){
        this->SERVOMIN = SERVOMIN;
        this->SERVOMAX = SERVMOMAX;
        this->ANGLEMIN = ANGLEMIN;
        this->ANGLEMAX = ANGLEMAX;
    }
    uint16_t SERVOMIN;
    uint16_t SERVOMAX;
    double ANGLEMIN;
    double ANGLEMAX;
};

class PCA9685Actuator : Actuator {
public:
    // constructor to set custom closed and open angle
    PCA9685Actuator(const std::string id, Adafruit_PWMServoDriver& pwm, uint channel, double closedAngle, double openAngle, PCA9685Config config);
    void setState(const bool state) override;
private:
    void rotateToAngle(double angle);
    uint16_t getPWMFromAngle(double angle);
    Adafruit_PWMServoDriver& pwm;
    uint8_t channel;
    double openAngle;
    double closedAngle;
    PCA9685Config config;
};

#endif