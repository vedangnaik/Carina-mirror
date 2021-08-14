#pragma once

#include <QVariantMap>
#include "Actuator.h"
#include "DummyActuator.h"
#include "PCA9685Actuator.h"

class ConcreteActuatorFactory {
public:
    static Actuator* createActuator(const std::string& id, const QVariantMap& args);
};