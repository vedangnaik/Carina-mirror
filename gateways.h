#ifndef CONTROLLERS_H
#define CONTROLLERS_H

#include "usecases.h"


struct ProcessData {
    std::vector<Sensor*> sensors;
    std::vector<Actuator*> actuators;
    std::vector<State*> states;
};

class ProcessGateway {
public:  
    struct ProcessData parseProcessFile(std::string fileName);
};

#endif // CONTROLLERS_H
