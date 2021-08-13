#ifndef PCA9685ACTUATOR_H
#define PCA9685ACTUATOR_H

#ifdef WIRINGPI_AVAILABLE

#include "AbstractActuator.h"
#include "Adafruit_PWMServoDriver.h"
#include <cstdint>
#include <cmath>
#include "easylogging++.h"
#include <cassert>
#include <type_traits>

class PCA9685Config{
public:
    PCA9685Config(uint16_t SERVOMIN, uint16_t SERVMOMAX, double ANGLEMIN, double ANGLEMAX){
        static_assert (SERVOMIN < SERVOMAX, "SERVOMIN must be less than SERVOMAX");
        static_assert (ANGLEMIN < ANGLEMAX, "ANGLEMIN must be less than ANGLEMAX");
        static_assert (0 <= ANGLEMIN && ANGLEMIN <= 360, "ANGLEMIN must be between 0 and 360 degrees");
        static_assert (0 <= ANGLEMAX && ANGLEMAX <= 360, "ANGLEMAX must be between 0 and 360 degrees");

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

#endif // WIRINGPI_AVAILABLE
#endif // PCA9685ACTUATOR_H
