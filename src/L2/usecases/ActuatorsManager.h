#pragma once

#include "Actuator.h"
#include "easylogging++.h"
#include <string>
#include <vector>
#include <stdexcept>
#include <QObject>

class AMOC {
public:
    virtual void notify(std::string id, bool status) = 0;
    virtual ~AMOC() = default;
};

class AMIC {
public:
    virtual bool getActuatorStatus(std::string id) = 0;
    virtual void setState(std::string id, bool status) = 0;
    virtual ~AMIC() = default;
};

class ActuatorsManager : public QObject, public AMIC {
    Q_OBJECT
public:
    ActuatorsManager(std::unordered_map<std::string, std::unique_ptr<Actuator>>& actuators, AMOC& amoc);
    bool getActuatorStatus(std::string id) override;
    void setState(std::string id, bool status) override;
private:
    std::unordered_map<std::string, std::unique_ptr<Actuator>> actuators;
    AMOC& amoc;
};


class ActuatorsManagerError : public std::runtime_error {
protected:
    explicit ActuatorsManagerError(const std::string& message) : std::runtime_error(message) {}
};

class NullptrActuatorError : public ActuatorsManagerError {
public:
    explicit NullptrActuatorError(const std::string& id) : ActuatorsManagerError("Actuator '" + id + "' is nullptr.") {}
};
