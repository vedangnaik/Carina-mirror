#ifndef USECASES_H
#define USECASES_H

#include "entities.h"


class ProcessManagerOutputContract {
public:
//    virtual void display(State* s) = 0;
    virtual ~ProcessManagerOutputContract() {};
};


struct StateDTO {
    std::string id;
    std::string name;
    std::string safetyRating;
    std::string description;
    std::vector<std::string> actions;
    std::string proceedState;
    std::string abortState;
};
class ProcessManager {
public:
    ProcessManager(ProcessManagerOutputContract* iucop);
    void createProcess(std::vector<StateDTO> Q);
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
