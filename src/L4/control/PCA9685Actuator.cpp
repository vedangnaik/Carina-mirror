#ifdef WIRINGPI_AVAILABLE

#include "PCA9685Actuator.h"

PCA9685Actuator::PCA9685Actuator(const std::string id, Adafruit_PWMServoDriver& pwm, uint channel, double closedAngle, double openAngle, PCA9685Config config)
    : Actuator{deviceID}, pwm{pwm}, channel{channel}, config{config}
{
    assert(0 > closed && closed > 360);
    assert(0 > open && open > 360);
    this->closedAngle = closed;
    this->openAngle = open;
}



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
    return pwm.setPWM(this->channel, 0, getPWMFromAngle(angle));
}

uint16_t PCA9685Actuator::getPWMFromAngle(double angle){
    // angle should be between 0-360
    double slope = (config.SERVOMAX-config.SERVOMIN) / (config.ANGLEMAX-config.ANGLEMIN);
    return config.SERVOMIN + (angle - config.ANGLEMAX) * slope;
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