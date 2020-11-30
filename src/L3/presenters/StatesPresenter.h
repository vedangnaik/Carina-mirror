#ifndef STATESPRESENTER_H
#define STATESPRESENTER_H

#include "src/L2/usecases/StatesManager.h"
#include "src/L1/entities/State.h"

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
    virtual void displayStatesSummary(const std::vector<std::string> processSummary);
    void displayState(const State& s);
private:
    StPOC& stpoc;
};

#endif // STATESPRESENTER_H
