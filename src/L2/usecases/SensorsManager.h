#ifndef SENSORMANAGER_H
#define SENSORMANAGER_H

#include "src/L1/entities/Sensor.h"
#include <map>
#include <stdexcept>

class SMOC {
public:
};

class SMIC {
public:
    virtual void addSensors(std::map<std::string, Sensor*> sensors) = 0;
    virtual float getSensorValue(std::string id) = 0;
    virtual void setSensorValue(std::string id, float value) = 0;
    virtual std::vector<std::string> getSensorIDs() = 0;
};

class SensorsManager : public SMIC {
public:
    void addSensors(std::map<std::string, Sensor*> sensors);
    float getSensorValue(std::string id);
    void setSensorValue(std::string id, float value);
    std::vector<std::string> getSensorIDs();
private:
    std::map<std::string, Sensor*> sensors;
    SMOC* smoc;
};

#endif // SENSORMANAGER_H
