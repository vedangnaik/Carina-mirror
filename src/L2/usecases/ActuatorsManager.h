#pragma once

#include "Actuator.h"
#include "easylogging++.h"
#include <map>
#include <stdexcept>

using std::string;
using std::map;

class AMOC {
public:
    virtual void notify(const string id, const bool status) = 0;
    virtual ~AMOC() {};
};

class AMIC {
public:
    virtual bool getActuatorStatus(string id) = 0;
    virtual void actuate(string id) = 0;
    virtual ~AMIC() {};
};

class ActuatorsManager : public AMIC {
public:
    ActuatorsManager(map<const string, Actuator> actuators);
    bool getActuatorStatus(string id);
    void actuate(string id);
    void setOutputContract(AMOC* amoc) {
        this->amoc = amoc;
    }
private:
    map<const string, Actuator> actuators;
    AMOC* amoc = nullptr;
};


class ActuatorsManagerError : public std::runtime_error {
protected:
    ActuatorsManagerError(string message) : std::runtime_error(message) {}
};

class NullptrActuatorError : public ActuatorsManagerError {
public:
    NullptrActuatorError(const string id) : ActuatorsManagerError("Actuator '" + id + "' is nullptr.") {}
};
