#include "ConcreteSensorFactory.h"

Sensor* ConcreteSensorFactory::createSensor(const std::string& id, const QVariantMap& args) {
    if (args["type"] == "DummyDAQ") {
        return new DummyDAQ(id);
    } else if (args["type"] == "OneWireDAQ") {
        // TODO Use QVariantMap to decipher this.
        // return new OneWireDAQ(id, ....)
        return nullptr;
    } else {
        // Whoops
        return nullptr;
    }
}