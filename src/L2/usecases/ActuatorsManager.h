#ifndef ACTUATORMANAGER_H
#define ACTUATORMANAGER_H

#include "src/L1/entities/Actuator.h"
#include <map>
#include <stdexcept>

class AMOC {
public:
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
    ActuatorsManager(std::map<std::string, Actuator*> actuators) : actuators(actuators) {};
    bool getActuatorStatus(std::string id);
    void actuate(std::string id);
private:
    const std::map<std::string, Actuator*> actuators;
    AMOC* amoc;
};

#endif // ACTUATORMANAGER_H
