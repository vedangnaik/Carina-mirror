#ifndef USECASES_H
#define USECASES_H

#include "entities.h"


class PMOC {
public:
    virtual void displayState(State* s) = 0;
    virtual ~PMOC() {};
};

class PMIC {
public:
    virtual void transition(std::string transition) = 0;
    virtual ~PMIC() {};
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
class ProcessManager : public PMIC {
public:
//    ProcessManager();
    void createProcess(std::vector<StateDTO> Q);
    void transition(std::string transition);

    Process* p = NULL;
    PMOC* pmoc;
};



class SensorsManager {
public:
    void createSensor(std::string id, std::string name);
    std::map<std::string, Sensor*> listOfSensors;
};

#endif // USECASES_H
