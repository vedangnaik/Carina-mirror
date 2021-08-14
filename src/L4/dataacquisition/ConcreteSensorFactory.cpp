#include "ConcreteSensorFactory.h"

Sensor* ConcreteSensorFactory::createSensor(const std::string id, const QVariantMap args) {
    if (id == "DummyDAQ") {
        return new DummyDAQ(id);
    } else if (id == "OneWireDAQ") {
        // TODO Use QVariantMap to decipher this.
        // return new OneWireDAQ(id, ....)
    } else {
        // Whoops
    }
}