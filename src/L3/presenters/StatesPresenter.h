#ifndef PROCESSPRESENTER_H
#define PROCESSPRESENTER_H

#include "src/L2/usecases/StatesManager.h"
#include "src/L1/entities/State.h"

class SPOC {
public:
    virtual void displayProcessSummary(std::vector<std::string> processSummary) = 0;
    virtual void displayState(const State& s) = 0;
    virtual void allowProceed(bool permission) = 0;
    virtual void allowAbort(bool permission) = 0;
};

class StatesPresenter : public StMOC {
public:
    StatesPresenter(SPOC* spoc);
    virtual void displayStatesSummary(std::vector<std::string> processSummary);
    void displayState(const State& s);
private:
    SPOC* spoc;
};

#endif // PROCESSPRESENTER_H
