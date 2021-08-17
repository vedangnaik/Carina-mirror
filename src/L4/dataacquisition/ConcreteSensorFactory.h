#pragma once

#include <QVariantMap>
#include <QJsonArray>
#include <unordered_map>
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
private:
    static std::array<std::pair<double, double>, 5> parseCalibrationPointsFromArgs(const QVariantMap& args);
    static Sensor* createDummySensor(const std::string& id, const QVariantMap& args);
    static Sensor* createAnalogMCCDAQSensor(const std::string& id, const QVariantMap& args);

    static void checkForKeyAndConversionValidity(const QVariantMap& args, const QString& key, int targetTypeId, const std::string& exceptionMsg);

    static std::unordered_map<std::string, Sensor* (*)(const std::string&, const QVariantMap&)> factoryMap;
};