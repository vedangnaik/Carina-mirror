#pragma once

#include "Helpers.h"
#include <QVariantMap>
#include "Actuator.h"
#include "DummyActuator.h"
#ifdef WIRINGPI_AVAILABLE
#include "PCA9685Actuator.h"
#include "Adafruit_PWMServoDriver.h"
#endif

class ConcreteActuatorFactory {
public:
    static Actuator* createActuator(const std::string& id, const QVariantMap& args);
    static void resetFactory();
private:
    static Actuator* createDummyActuator(const std::string& id, const QVariantMap& args);
    static Actuator* createPCA9685Actuator(const std::string& id, const QVariantMap& args);
#ifdef WIRINGPI_AVAILABLE
    static std::unordered_map<unsigned int, Adafruit_PWMServoDriver*> cachedServoDriverBoards;
#endif

    static std::unordered_map<std::string, Actuator* (*)(const std::string&, const QVariantMap&)> factoryMap;
};