#ifndef PROCESSPRESENTER_H
#define PROCESSPRESENTER_H

#include "src/L2/usecases/ProcessManager.h"
#include "src/L1/entities/State.h"

class PPOC {
public:
    virtual void displayProcessSummary(std::vector<std::string> processSummary) = 0;
    virtual void displayState(const State& s) = 0;
    virtual void allowProceed(bool permission) = 0;
    virtual void allowAbort(bool permission) = 0;
};

class ProcessPresenter : public PMOC {
public:
    ProcessPresenter(PPOC* ppoc);
    virtual void displayProcessSummary(std::vector<std::string> processSummary);
    void displayState(const State& s);
private:
    PPOC* ppoc;
};

#endif // PROCESSPRESENTER_H
