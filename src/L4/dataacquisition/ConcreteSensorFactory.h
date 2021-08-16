#pragma once

#include <QVariantMap>
#include <QJsonArray>
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
};