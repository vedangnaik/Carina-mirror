#pragma once

#include "Actuator.h"
#include "easylogging++.h"
#include <string>
#include <vector>
#include <stdexcept>
#include <QObject>

class AMOC {
public:
    virtual void notify(const std::string id, const bool status) = 0;
    virtual ~AMOC() {};
};

class AMIC {
public:
    virtual bool getActuatorStatus(std::string id) = 0;
    virtual void setState(std::string id, bool status) = 0;
    virtual ~AMIC() {};
};

class ActuatorsManager : public QObject, public AMIC {
    Q_OBJECT
public:
    ActuatorsManager(std::vector<Actuator*> actuators, AMOC& amoc);
    bool getActuatorStatus(std::string id) override;
    void setState(std::string id, bool status) override;
private:
    std::vector<Actuator*> actuators;
    AMOC& amoc;
};


class ActuatorsManagerError : public std::runtime_error {
protected:
    ActuatorsManagerError(std::string message) : std::runtime_error(message) {}
};

class NullptrActuatorError : public ActuatorsManagerError {
public:
    NullptrActuatorError(const std::string id) : ActuatorsManagerError("Actuator '" + id + "' is nullptr.") {}
};
