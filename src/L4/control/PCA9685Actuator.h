#ifndef PCA9685ACTUATOR_H
#define PCA9685ACTUATOR_H

#include "AbstractActuator.h"
#include "Adafruit_PWMServoDriver.h"
#include <cstdint>
#include <cmath>
#include "easylogging++.h"

class PCA9685Config{
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

class PCA9685Actuator : AbstractActuator {
public:
    // "default" constructor: sets closed angle to 0, open angle to 90 degrees
    PCA9685Actuator(const std::string deviceID, Adafruit_PWMServoDriver& pwm, uint channel, PCA9685Config config);
    // constructor to set custom closed and open angle
    PCA9685Actuator(const std::string deviceID, Adafruit_PWMServoDriver& pwm, uint channel, double closed, double open, PCA9685Config config);
    void toggle() override;
    void setState(const bool state) override;
    void rotateToAngle(double angle);

private:
    uint16_t getPWMFromAngle(double angle);
    Adafruit_PWMServoDriver& pwm;
    uint8_t channel;
    double openAngle;
    double closedAngle;
    PCA9685Config config;
};

#endif // PCA9685ACTUATOR_H
