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
    virtual void transition(Transition t, bool override = false) = 0;
    virtual ~StMIC() {};
};

class StatesManager : public StMIC {
public:
    StatesManager(map<const string, const State> states, SMIC& smic, AMIC& amic);
    void transition(Transition t, bool override = false) override;
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



// This is the base class of all exceptions that can occur with the StateManager class
// Subclass a new exception from it if a new bug/error needs to be handled.
class StatesManagerError : public std::runtime_error {
protected:
    StatesManagerError(string message) : std::runtime_error(message) {}
};

class NoStartStateError : public StatesManagerError {
public:
    NoStartStateError() : StatesManagerError("No 'start' state found in process file") {}
};
