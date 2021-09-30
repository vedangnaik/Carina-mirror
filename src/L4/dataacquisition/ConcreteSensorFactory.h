#pragma once

#include <QVariantMap>
#include <QJsonArray>
#include <unordered_map>
#include <functional>
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
    ConcreteSensorFactory();
    ~ConcreteSensorFactory();
    Sensor* createSensor(const std::string& id, const QVariantMap& args);
private:
    Sensor* createDummySensor(const std::string& id, const QVariantMap& args);
#ifdef ULDAQ_AVAILABLE
    Sensor* createAnalogMCCDAQSensor(const std::string& id, const QVariantMap& args);
    void discoverAndConnectToMCCDAQs();
    void disconnectFromMCCDAQs();
    std::vector<DaqDeviceHandle> cachedMCCDAQs;
#endif
};