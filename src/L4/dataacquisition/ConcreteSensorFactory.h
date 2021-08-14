#pragma once

#include <QVariantMap>
#include "Sensor.h"
#include "DummyDAQ.h"
#include "OneWireDAQ.h"
// etc.

class ConcreteSensorFactory {
public:
    static Sensor* createSensor(const std::string& id, const QVariantMap& args);
};