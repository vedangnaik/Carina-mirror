#pragma once

#include "State.h"
#include "SensorsManager.h"
#include "ActuatorsManager.h"

using std::string;
using std::map;

class StMOC {
public:
    virtual void displayStatesSummary(const std::vector<string> processSummary) = 0;
    virtual void displayState(const State& s) = 0;
    virtual void displayFailedChecks(const std::vector<std::string> failures, Transition t) = 0;
    virtual ~StMOC() {};
};

class StMIC {
public:
    virtual void transition(Transition t) = 0;
    virtual ~StMIC() {};
};

class StatesManager : public StMIC {
public:
    StatesManager(map<const string, const State> states, SMIC& smic, AMIC& amic);
    void transition(Transition t);
    void setOutputContract(StMOC* stmoc) {
        this->stmoc = stmoc;
    }
    void startProcess();
    void stopProcess();
private:
    map<const string, const State> states;
    const State* currentState;

    StMOC* stmoc;
    SMIC& smic;
    AMIC& amic;
};
