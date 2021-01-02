#pragma once

#include "Actuator.h"
#include "easylogging++.h"
#include <map>
#include <stdexcept>

class AMOC {
public:
    virtual void notify(const std::string id, const bool status) = 0;
    virtual ~AMOC() {};
};

class AMIC {
public:
    virtual bool getActuatorStatus(std::string id) = 0;
    virtual void actuate(std::string id) = 0;
    virtual ~AMIC() {};
};

class ActuatorsManager : public AMIC {
public:
    ActuatorsManager(std::map<std::string, Actuator*> actuators);
    bool getActuatorStatus(std::string id);
    void actuate(std::string id);
    void setOutputContract(AMOC* amoc) {
        this->amoc = amoc;
    }
private:
    const std::map<std::string, Actuator*> actuators;
    AMOC* amoc = nullptr;
};


class ActuatorsManagerError : public std::runtime_error {
protected:
    ActuatorsManagerError(std::string message) : std::runtime_error(message) {}
};

class NullptrActuatorError : public ActuatorsManagerError {
public:
    NullptrActuatorError(const std::string id) : ActuatorsManagerError("Actuator '" + id + "' is nullptr.") {}
};
