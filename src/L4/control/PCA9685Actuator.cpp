#ifdef WIRINGPI_AVAILABLE

#include "PCA9685Actuator.h"
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::milliseconds

PCA9685Actuator(std::string id, uint8_t channel, double openAngle, double closedAngle,
                double overdriveAngle, int overdriveDelay,
                uint16_t SERVOMIN, uint16_t SERVMOMAX,
                double ANGLEMIN, double ANGLEMAX,
                std::shared_ptr<Adafruit_PWMServoDriver> pwm)
    : Actuator(std::move(id)),
      channel{channel},
      openAngle{openAngle}, closedAngle{closedAngle},
      overdriveAngle{overdriveAngle}, overdriveDelay{overdriveDelay},
      SERVOMIN{SERVOMIN}, SERVOMAX{SERVMOMAX},
      ANGLEMIN{ANGLEMIN}, ANGLEMAX{ANGLEMAX},
      pwm{std::move(pwm)}
{}

void PCA9685Actuator::setState(const bool state) {
    this->state = state;
    if (this->state){
        if (openAngle > closedAngle) {
            rotateOverdrive(openAngle, overdriveAngle);
        } else {
            rotateOverdrive(openAngle, 0.0);
        }
        LOG(DEBUG) << "PCA9685 channel " << this->channel << " servo: state set to OPEN.";
    } else {
        if (closedAngle > openAngle){
            rotateOverdrive(closedAngle, overdriveAngle);
        } else {
            rotateOverdrive(closedAngle, 0.0);
        }
        LOG(DEBUG) << "PCA9685 channel " << this->channel << " servo: state set to CLOSED.";
    }
}

void PCA9685Actuator::rotateOverdrive(double angle, double overAngle){
    rotateToAngle(overAngle);
    std::this_thread::sleep_for(std::chrono::milliseconds(overdriveDelay));
    rotateToAngle(angle);
}

void PCA9685Actuator::rotateToAngle(double angle) {
    angle = std::fmod(angle, 360);
    if (angle < 0) {
        angle = 360 + angle;
    }
    LOG(DEBUG) << this->id << ": Servo rotated to " << angle << " degrees.";
    return pwm->setPWM(this->channel, 0, getPWMFromAngle(angle));
}

uint16_t PCA9685Actuator::getPWMFromAngle(double angle) const {
    double slope = (this->SERVOMAX - this->SERVOMIN) / (this->ANGLEMAX - this->ANGLEMIN);
    double PWMAngle = (angle - this->ANGLEMIN) * slope + this->SERVOMIN;
    LOG(DEBUG) << this->id << ": Servo PWM for " << angle << " degrees is " << PWMAngle << " (out of 4096).";
    return PWMAngle;
}

#endif
