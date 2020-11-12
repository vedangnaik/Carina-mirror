#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include "src/entities/State.h"
#include "src/entities/Process.h"
#include "ActuatorsManager.h"
#include "SensorsManager.h"

class PMOC {
public:
    virtual void displayProcessSummary(std::vector<std::string> processSummary) = 0;;
    virtual void displayState(State* s) = 0;
    virtual ~PMOC() {};
};

class PMIC {
public:
    virtual void createProcess(std::map<std::string, State*> states) = 0;
    virtual void startProcess() = 0;
    virtual void transition(Transition t) = 0;
    virtual ~PMIC() {};
};

class ProcessManager : public PMIC {
public:
    ProcessManager(AMIC* amic, SMIC* smic);
    void createProcess(std::map<std::string, State*> states);
    void startProcess();
    void transition(Transition t);
    void setOutputContract(PMOC* pmoc);
private:
    Process* p = NULL;
    PMOC* pmoc;
    AMIC* amic;
    SMIC* smic;
};

#endif // PROCESSMANAGER_H
