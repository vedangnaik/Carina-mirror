#ifndef CONTROLLERS_H
#define CONTROLLERS_H

#include "src/entities/Actuator.h"
#include "src/entities/State.h"
#include "src/entities/Sensor.h"

#include <QJsonObject>

struct ProcessData {
    std::vector<Sensor*> sensors;
    std::vector<Actuator*> actuators;
    std::vector<State*> states;
};

class ProcessGateway {
public:  
    struct ProcessData parseProcessFile(std::string fileName);
private:
    std::vector<Sensor*> parseSensors(QJsonObject sensorsObj);
    std::vector<Actuator*> parseActuators(QJsonObject actuatorsObj);
    std::vector<State*> parseStates(QJsonObject statesObj);
    std::map<std::string, std::vector<unsigned int>> parseStateChecks(QJsonObject checksObj);
};

#endif // CONTROLLERS_H
