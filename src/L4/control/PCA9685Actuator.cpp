#include "PCA9685Actuator.h"

PCA9685Actuator::PCA9685Actuator(const std::string deviceID, Adafruit_PWMServoDriver& pwm, uint channel, PCA9685Config config)
    : AbstractActuator(deviceID), pwm(pwm), channel(channel), config(config)
{
    this->closedAngle = 0.0; // this may need to be set lower to ensure its closing?
    this->openAngle = 90.0; // this may need to be set higher?
}

PCA9685Actuator::PCA9685Actuator(const std::string deviceID, Adafruit_PWMServoDriver& pwm, uint channel, double closed, double open, PCA9685Config config)
    : AbstractActuator(deviceID), pwm(pwm), channel(channel), openAngle(open), closedAngle(closed), config(config) {}

void PCA9685Actuator::toggle() {
    if (this->state == OPEN){
        rotateToAngle(openAngle);
    }
    else {
        rotateToAngle(closedAngle);
    }
    this->state = !this->state;
}

void PCA9685Actuator::setState(const bool state) {
    if (state == OPEN){
        rotateToAngle(openAngle);
    }
    else{
        rotateToAngle(closedAngle);
    }
    this->state = state;
}

void PCA9685Actuator::rotateToAngle(double angle) {
    angle = std::fmod(angle, 360);
    if (angle < 0)
        angle = 360 + angle;
    return pwm.setPWM(this->channel, 0, getPWMFromAngle(angle));
}

uint16_t PCA9685Actuator::getPWMFromAngle(double angle){
    // angle should be between 0-360
    double slope = (config.SERVOMAX-config.SERVOMIN) / (config.ANGLEMAX-config.ANGLEMIN);
    return config.SERVOMIN + (angle - config.ANGLEMAX) * slope;
}
