#ifndef ACTUATORMANAGER_H
#define ACTUATORMANAGER_H

#include "src/entities/Actuator.h"
#include <vector>
#include <map>
#include <stdexcept>

class AMOC {
public:
};

class AMIC {
public:
    virtual void addActuators(std::map<std::string, Actuator*> actuators) = 0;
    virtual bool getActuatorStatus(std::string id) = 0;
    virtual void actuate(std::string id) = 0;
};

class ActuatorsManager : public AMIC {
public:
    void addActuators(std::map<std::string, Actuator*> actuators);
    bool getActuatorStatus(std::string id);
    void actuate(std::string id);
private:
    std::map<std::string, Actuator*> actuators;
    AMOC* amoc;
};

#endif // ACTUATORMANAGER_H
