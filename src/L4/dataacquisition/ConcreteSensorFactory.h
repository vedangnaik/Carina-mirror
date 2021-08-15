#pragma once

#include <QVariantMap>
#include <QJsonArray>
#include "Sensor.h"
#include "DummySensor.h"
#include "AnalogMCCDAQSensor.h"
#ifdef ULDAQ_AVAILABLE
#include <uldaq.h>
#endif

class ConcreteSensorFactory {
public:
    static Sensor* createSensor(const std::string& id, const QVariantMap& args);
private:
    static std::array<std::pair<double, double>, 5> parseCalibrationPointsFromArgs(const QVariantMap& args);
};