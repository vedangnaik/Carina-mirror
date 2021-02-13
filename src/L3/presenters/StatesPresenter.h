#pragma once

#include "State.h"
#include "StatesManager.h"

class StPOC {
public:
    virtual void displayProcessSummary(const std::vector<std::string> processSummary) = 0;
    virtual void displayState(const State& s) = 0;
    virtual void allowProceed(bool permission) = 0;
    virtual void allowAbort(bool permission) = 0;
    virtual void allowFailedChecksOverride(const std::vector<std::string> failures, Transition t) = 0;
    virtual ~StPOC() {};
};

class StatesPresenter : public StMOC {
public:
    StatesPresenter(StPOC& stpoc) : stpoc(stpoc){};
    void displayStatesSummary(const std::vector<std::string> processSummary) override;
    void displayState(const State& s) override;
    void displayFailedChecks(const std::vector<std::string> failures, Transition t) override;
private:
    StPOC& stpoc;
};
