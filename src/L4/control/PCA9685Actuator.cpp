#ifdef WIRINGPI_AVAILABLE

#include "PCA9685Actuator.h"

PCA9685Actuator::PCA9685Actuator(std::string id, uint8_t channel, double openAngle, double closedAngle, uint16_t SERVOMIN, uint16_t SERVMOMAX, double ANGLEMIN, double ANGLEMAX, std::shared_ptr<Adafruit_PWMServoDriver> pwm)
    : Actuator(std::move(id)), channel{channel}, openAngle{openAngle}, closedAngle{closedAngle}, SERVOMIN{SERVOMIN}, SERVOMAX{SERVMOMAX}, ANGLEMIN{ANGLEMIN}, ANGLEMAX{ANGLEMAX}, pwm{std::move(pwm)}
{}

void PCA9685Actuator::setState(const bool state) {
    this->state = state;
    if (this->state) {
        rotateToAngle(openAngle);
    } else {
        rotateToAngle(closedAngle);
    }
}

void PCA9685Actuator::rotateToAngle(double angle) {
    angle = std::fmod(angle, 360);
    if (angle < 0) {
        angle = 360 + angle;
    }
    return pwm->setPWM(this->channel, 0, getPWMFromAngle(angle));
}

uint16_t PCA9685Actuator::getPWMFromAngle(double angle) const {
    // angle should be between 0-360
    double slope = (this->SERVOMAX - this->SERVOMIN) / (this->ANGLEMAX - this->ANGLEMIN);
    return this->SERVOMIN + (angle - this->ANGLEMAX) * slope;
}

#endif






// PCA9685Actuator::PCA9685Actuator(const std::string deviceID, Adafruit_PWMServoDriver& pwm, uint channel, PCA9685Config config)
//     : Actuator(deviceID), pwm(pwm), channel(channel), config(config)
// {
//     this->closedAngle = 0.0; // this may need to be set lower to ensure its closing?
//     this->openAngle = 90.0; // this may need to be set higher?
// }

// void PCA9685Actuator::toggle() {
//     if (this->state == OPEN){
//         rotateToAngle(closedAngle);
//     }
//     else {
//         rotateToAngle(openAngle);
//     }
//     this->state = !this->state;
// }