#pragma once

#include <QVariantMap>
#include <QJsonArray>
#include <unordered_map>
#include "Helpers.h"
#include "easylogging++.h"
#include "Sensor.h"
#include "DummySensor.h"
#ifdef ULDAQ_AVAILABLE
#include "AnalogMCCDAQSensor.h"
#include <uldaq.h>
#endif

class ConcreteSensorFactory {
public:
    static Sensor* createSensor(const std::string& id, const QVariantMap& args);
    static void resetFactory();
private:
    static Sensor* createDummySensor(const std::string& id, const QVariantMap& args);
    static Sensor* createAnalogMCCDAQSensor(const std::string& id, const QVariantMap& args);

    static std::unordered_map<std::string, Sensor* (*)(const std::string&, const QVariantMap&)> factoryMap;
};