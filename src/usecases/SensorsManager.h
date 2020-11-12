#ifndef SENSORMANAGER_H
#define SENSORMANAGER_H

#include "src/entities/Sensor.h"
#include <map>
#include <stdexcept>

class SMOC {
public:
};

class SMIC {
public:
    virtual void addSensors(std::vector<Sensor*> sensors) = 0;
    virtual Sensor* findSensor(std::string id) = 0;
};

class SensorsManager : public SMIC {
public:
    void addSensors(std::vector<Sensor*> sensors);
    Sensor* findSensor(std::string id);
private:
    std::map<std::string, Sensor*> sensors;
    SMOC* smoc;
};

#endif // SENSORMANAGER_H
