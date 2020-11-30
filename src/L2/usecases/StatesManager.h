#ifndef STATESMANAGER_H
#define STATESMANAGER_H

#include "src/L1/entities/State.h"
#include "src/L2/usecases/SensorsManager.h"
#include "src/L2/usecases/ActuatorsManager.h"
#include "src/L2/services/ClocksModule.h"

class StMOC {
public:
    virtual void displayStatesSummary(const std::vector<std::string> processSummary) = 0;
    virtual void displayState(const State& s) = 0;
};

class StMIC {
public:
    virtual void transition(Transition t) = 0;
};

class StatesManager : public StMIC {
public:
    StatesManager(std::map<std::string, State*> states, SMIC& smic,
                  AMIC& amic) : states(states), smic(smic), amic(amic) {};
    void transition(Transition t);
    void setOutputContract(StMOC* stmoc) {
        this->stmoc = stmoc;
    }
    void startProcess();
    void stopProcess();
private:
    const std::map<std::string, State*> states;
    State* currentState;
    bool inProgress = false;

    StMOC* stmoc;
    SMIC& smic;
    AMIC& amic;
};

#endif // STATESMANAGER_H
