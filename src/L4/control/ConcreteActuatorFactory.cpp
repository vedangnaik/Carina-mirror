#include "ConcreteActuatorFactory.h"

Actuator* ConcreteActuatorFactory::createActuator(const std::string& id, const QVariantMap& args) {
    if (args["type"] == "DummyActuator") {
        return new DummyActuator(id);
    } else if (args["type"] == "PCA9685Actuator") {
        // TODO Use QVariantMap to decipher this.
        // return new OneWireDAQ(id, ....)
        return nullptr;
    } else {
        // Whoops
        return nullptr;
    }
}