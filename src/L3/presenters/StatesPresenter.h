#ifndef STATESPRESENTER_H
#define STATESPRESENTER_H

#include "State.h"
#include "StatesManager.h"

class StPOC {
public:
    virtual void displayProcessSummary(const std::vector<std::string> processSummary) = 0;
    virtual void displayState(const State& s) = 0;
    virtual void allowProceed(bool permission) = 0;
    virtual void allowAbort(bool permission) = 0;
    virtual ~StPOC() {};
};

class StatesPresenter : public StMOC {
public:
    StatesPresenter(StPOC& stpoc) : stpoc(stpoc){};
    void displayStatesSummary(const std::vector<std::string> processSummary);
    void displayState(const State& s);
private:
    StPOC& stpoc;
};

#endif // STATESPRESENTER_H
