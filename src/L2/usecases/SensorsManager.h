#pragma once

#include "Sensor.h"
#include "easylogging++.h"
#include <map>
#include <stdexcept>



class SMOC {
public:
    virtual void notify(const std::string id, const float value) = 0;
    virtual ~SMOC() {};
};

class SMIC {
public:
    virtual float getSensorValue(std::string id) = 0;
    virtual void setSensorValue(std::string id, float value) = 0;
    virtual std::vector<std::string> getSensorIDs() = 0;
    virtual ~SMIC() {};
private:

};

class SensorsManager : public SMIC {
public:
    SensorsManager(std::map<const std::string, Sensor> sensors);
    float getSensorValue(std::string id);
    void setSensorValue(std::string id, float value);
    std::vector<std::string> getSensorIDs();
    void setOutputContract(SMOC* smoc) {
        this->smoc = smoc;
    }
private:
    std::map<const std::string, Sensor> sensors;
    SMOC* smoc;
};



class SensorsManagerError : public std::runtime_error {
protected:
    SensorsManagerError(std::string message) : std::runtime_error(message) {}
};

class NullptrSensorError : public SensorsManagerError {
public:
    NullptrSensorError(const std::string id) : SensorsManagerError("Sensor '" + id + "' is nullptr.") {}
};

