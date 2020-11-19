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
                  AMIC& amic, ClocksModule& cm) : states(states), smic(smic), amic(amic), cm(cm) {};
    void transition(Transition t);
    void setOutputContract(StMOC* stmoc) {
        this->stmoc = stmoc;
    }
    void startProcess();
private:
    const std::map<std::string, State*> states;
    State* currentState;

    StMOC* stmoc;
    SMIC& smic;
    AMIC& amic;
    ClocksModule& cm;
};

#endif // STATESMANAGER_H
