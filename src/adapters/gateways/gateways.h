#ifndef CONTROLLERS_H
#define CONTROLLERS_H

#include "src/usecases/usecases.h"

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
    std::vector<Actuator*> parseActuator(QJsonObject actuatorsObj);
    std::vector<State*> parseState(QJsonObject statesObj);
};

#endif // CONTROLLERS_H
