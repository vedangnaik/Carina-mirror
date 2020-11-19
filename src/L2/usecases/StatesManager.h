#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include "src/L1/entities/State.h"
#include "ActuatorsManager.h"
#include "SensorsManager.h"
#include "src/L2/services/ClocksModule.h"

class StMOC {
public:
    virtual void displayStatesSummary(std::vector<std::string> processSummary) = 0;
    virtual void displayState(const State& s) = 0;
};

class StMIC {
public:
    virtual void createProcess(std::map<std::string, Sensor*> sensors, std::map<std::string, Actuator*> actuators, std::map<std::string, State*> states) = 0;
    virtual void startProcess() = 0;
    virtual void transition(Transition t) = 0;
};

class StatesManager : public StMIC {
public:
    StatesManager(AMIC* amic, SMIC* smic, ClocksModule* cm);
    void createProcess(std::map<std::string, Sensor*> sensors, std::map<std::string, Actuator*> actuators, std::map<std::string, State*> states);
    void startProcess();
    void transition(Transition t);
    void setOutputContract(StMOC* pmoc);
private:
    std::map<std::string, State*> states;
    State* currentState;
    StMOC* pmoc;
    AMIC* amic;
    SMIC* smic;
    ClocksModule* cm;
};

#endif // PROCESSMANAGER_H
