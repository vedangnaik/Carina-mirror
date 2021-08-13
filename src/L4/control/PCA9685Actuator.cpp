#ifdef WIRINGPI_AVAILABLE

#include "PCA9685Actuator.h"

PCA9685Actuator::PCA9685Actuator(const std::string deviceID, Adafruit_PWMServoDriver& pwm, uint channel, PCA9685Config config)
    : AbstractActuator(deviceID), pwm(pwm), channel(channel), config(config)
{
    this->closedAngle = 0.0; // this may need to be set lower to ensure its closing?
    this->openAngle = 90.0; // this may need to be set higher?
    LOG(DEBUG) << "PCA9685 channel " << this->channel << " servo: closed angle is " << this->closedAngle " degrees";
    LOG(DEBUG) << "PCA9685 channel " << this->channel << " servo: open angle is " << this->openAngle " degrees";
    LOG(INFO) << "PCA9685 channel " << this->channel << " servo: initial state is CLOSED";
}

PCA9685Actuator::PCA9685Actuator(const std::string deviceID, Adafruit_PWMServoDriver& pwm, uint channel, double closed, double open, PCA9685Config config)
    : AbstractActuator(deviceID), pwm(pwm), channel(channel), config(config) {
    assert (0 > closed || closed > 360);
    assert (0 > open || open > 360);
    this->closedAngle = closed;
    this->openAngle = open;
    LOG(DEBUG) << "PCA9685 channel " << this->channel << " servo: closed angle is " << this->closedAngle " degrees";
    LOG(DEBUG) << "PCA9685 channel " << this->channel << " servo: open angle is " << this->openAngle " degrees";
    LOG(INFO) << "PCA9685 channel " << this->channel << " servo: initial state is CLOSED";
}

void PCA9685Actuator::toggle() {
    if (this->state == OPEN){
        rotateToAngle(closedAngle);
        LOG(INFO) << "PCA9685 channel " << this->channel << " servo: state toggled from OPEN to CLOSED";
    }
    else {
        rotateToAngle(openAngle);
        LOG(INFO) << "PCA9685 channel " << this->channel << " servo: state toggled from CLOSED to OPEN";
    }
    this->state = !this->state;
}

void PCA9685Actuator::setState(const bool state) {
    if (state == OPEN){
        rotateToAngle(openAngle);
        LOG(INFO) << "PCA9685 channel " << this->channel << " servo: state set to OPEN";
    }
    else{
        rotateToAngle(closedAngle);
        LOG(INFO) << "PCA9685 channel " << this->channel << " servo: state set to CLOSED";
    }
    this->state = state;
}

void PCA9685Actuator::rotateToAngle(double angle) {
    angle = std::fmod(angle, 360);
    if (angle < 0)
        angle = 360 + angle;
    LOG(DEBUG) << "PCA9685 channel " << this->channel << " servo: rotated to " << angle << " degrees";
    return pwm.setPWM(this->channel, 0, getPWMFromAngle(angle));
}

uint16_t PCA9685Actuator::getPWMFromAngle(double angle){
    // angle should be between 0-360
    double slope = (config.SERVOMAX-config.SERVOMIN) / (config.ANGLEMAX-config.ANGLEMIN);
    PWM = config.SERVOMIN + (angle - config.ANGLEMAX) * slope
    LOG(DEBUG) << "PCA9685 channel " << this->channel << " servo: PWM (out of 4096) for " << angle << " degrees is " << PWM;
    return PWM;
}

#endif
