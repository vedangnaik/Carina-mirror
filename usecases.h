#ifndef USECASES_H
#define USECASES_H

#include "entities.h"


class ProcessManagerOutputContract {
public:
//    virtual void display(State* s) = 0;
    virtual ~ProcessManagerOutputContract() {};
};


class ProcessManager {
public:
    ProcessManager(ProcessManagerOutputContract* iucop);
    void transition(std::string alphabet);

private:
    Process* p = NULL;
    ProcessManagerOutputContract* icuop;
};



class SensorsManager {
public:
    void createSensor(std::string id, std::string name);
    std::map<std::string, Sensor*> listOfSensors;
};

#endif // USECASES_H
