#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include "src/L1/entities/State.h"
#include "src/L1/entities/Process.h"
#include "ActuatorsManager.h"
#include "SensorsManager.h"
#include "src/L2/services/ClocksModule.h"

class PMOC {
public:
    virtual void displayProcessSummary(std::vector<std::string> processSummary) = 0;;
    virtual void displayState(State* s) = 0;
};

class PMIC {
public:
    virtual void createProcess(std::map<std::string, Sensor*> sensors, std::map<std::string, Actuator*> actuators, std::map<std::string, State*> states) = 0;
    virtual void startProcess() = 0;
    virtual void transition(Transition t) = 0;
};

class ProcessManager : public PMIC {
public:
    ProcessManager(AMIC* amic, SMIC* smic, ClocksModule* cm);
    void createProcess(std::map<std::string, Sensor*> sensors, std::map<std::string, Actuator*> actuators, std::map<std::string, State*> states);
    void startProcess();
    void transition(Transition t);
    void setOutputContract(PMOC* pmoc);
private:
    Process* p = NULL;
    PMOC* pmoc;
    AMIC* amic;
    SMIC* smic;
    ClocksModule* cm;
};

#endif // PROCESSMANAGER_H
