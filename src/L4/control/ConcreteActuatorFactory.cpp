#include "ConcreteActuatorFactory.h"

Actuator* ConcreteActuatorFactory::createActuator(const std::string& id, const QVariantMap args) {
    if (id == "DummyActuator") {
        return new DummyActuator(id);
    } else if (id == "PCA9685Actuator") {
        // TODO Use QVariantMap to decipher this.
        // return new OneWireDAQ(id, ....)
        return nullptr;
    } else {
        // Whoops
        return nullptr;
    }
}