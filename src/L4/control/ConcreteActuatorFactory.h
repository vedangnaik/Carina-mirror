#pragma once

#include "Helpers.h"
#include <QVariantMap>
#include "Actuator.h"
#include "DummyActuator.h"
#ifdef WIRINGPI_AVAILABLE
#include "SolenoidActuator.h"
#include <wiringPi.h>
#include "PCA9685Actuator.h"
#include "Adafruit_PWMServoDriver.h"
#endif

class ConcreteActuatorFactory {
public:
    Actuator* createActuator(const std::string& id, const QVariantMap& args);
private:
    Actuator* createDummyActuator(const std::string& id, const QVariantMap& args);
#ifdef WIRINGPI_AVAILABLE
    Actuator* createPCA9685Actuator(const std::string& id, const QVariantMap& args);
    Actuator* createSolenoidActuator(const std::string& id, const QVariantMap& args);
    std::unordered_map<unsigned int, Adafruit_PWMServoDriver*> cachedServoDriverBoards;
#endif
};