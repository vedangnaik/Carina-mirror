#pragma once

#include "Sensor.h"
#include "easylogging++.h"
#include <map>
#include <stdexcept>

using std::string;
using std::map;

class SMOC {
public:
    virtual void notify(const string id, const float value) = 0;
    virtual ~SMOC() {};
};

class SMIC {
public:
    virtual float getSensorValue(string id) = 0;
    virtual void setSensorValue(string id, float value) = 0;
    virtual std::vector<string> getSensorIDs() = 0;
    virtual ~SMIC() {};
private:

};

class SensorsManager : public SMIC {
public:
    SensorsManager(map<const string, Sensor> sensors);
    float getSensorValue(string id);
    void setSensorValue(string id, float value);
    std::vector<string> getSensorIDs();
    void setOutputContract(SMOC* smoc) {
        this->smoc = smoc;
    }
private:
    map<const string, Sensor> sensors;
    SMOC* smoc;
};



class SensorsManagerError : public std::runtime_error {
protected:
    SensorsManagerError(string message) : std::runtime_error(message) {}
};

class NullptrSensorError : public SensorsManagerError {
public:
    NullptrSensorError(const string id) : SensorsManagerError("Sensor '" + id + "' is nullptr.") {}
};

