#ifndef CONTROLLERS_H
#define CONTROLLERS_H

#include "src/entities/Actuator.h"
#include "src/entities/State.h"
#include "src/entities/Sensor.h"

#include <QJsonObject>

struct ProcessData {
    std::map<std::string, Sensor*> sensors;
    std::map<std::string, Actuator*> actuators;
    std::map<std::string, State*> states;
};

class ProcessGateway {
public:  
    struct ProcessData parseProcessFile(std::string fileName);
private:
    std::map<std::string, Sensor*> parseSensors(QJsonObject sensorsObj);
    std::map<std::string, Actuator*> parseActuators(QJsonObject actuatorsObj);
    std::map<std::string, State*> parseStates(QJsonObject statesObj, std::map<std::string, Sensor*> sensors, std::map<std::string, Actuator*> actuators);
    void parseStateChecks(QJsonValue checks, std::map<std::string, Sensor*> sensors, std::map<std::string, Actuator*> actuators, State* s, Transition t);
};

#endif // CONTROLLERS_H
