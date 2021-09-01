#ifdef WIRINGPI_AVAILABLE

#include "PCA9685Actuator.h"

PCA9685Actuator::PCA9685Actuator(std::string id, uint8_t channel, double openAngle, double closedAngle, uint16_t SERVOMIN, uint16_t SERVMOMAX, double ANGLEMIN, double ANGLEMAX, std::shared_ptr<Adafruit_PWMServoDriver> pwm)
    : Actuator(std::move(id)), channel{channel}, openAngle{openAngle}, closedAngle{closedAngle}, SERVOMIN{SERVOMIN}, SERVOMAX{SERVMOMAX}, ANGLEMIN{ANGLEMIN}, ANGLEMAX{ANGLEMAX}, pwm{std::move(pwm)}
{}

void PCA9685Actuator::setState(const bool state) {
    this->state = state;
    if (this->state) {
        rotateToAngle(openAngle);
        LOG(INFO) << "PCA9685 channel " << this->channel << " servo: state set to OPEN.";
    } else {
        rotateToAngle(closedAngle);
        LOG(INFO) << "PCA9685 channel " << this->channel << " servo: state set to CLOSED.";
    }
}

void PCA9685Actuator::rotateToAngle(double angle) {
    angle = std::fmod(angle, 360);
    if (angle < 0) {
        angle = 360 + angle;
    }
    LOG(INFO) << this->id << ": Servo rotated to " << angle << " degrees.";
    return pwm->setPWM(this->channel, 0, getPWMFromAngle(angle));
}

uint16_t PCA9685Actuator::getPWMFromAngle(double angle) const {
    // angle should be between 0-360
    double slope = (this->SERVOMAX - this->SERVOMIN) / (this->ANGLEMAX - this->ANGLEMIN);
    double PWMAngle = this->SERVOMIN + (angle - this->ANGLEMAX) * slope;
    LOG(INFO) << this->id << ": Servo PWM for " << angle << " degrees is " << PWMAngle << " (out of 4096).";
    return PWMAngle;
}

#endif