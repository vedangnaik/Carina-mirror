#ifndef PROCESSPRESENTER_H
#define PROCESSPRESENTER_H

#include "src/usecases/usecases.h"

class PPOC {
public:
    virtual void displayProcessSummary(std::vector<std::string> processSummary) = 0;
    virtual void displayState(State* s) = 0;
    virtual void toggleProceed(bool yes) = 0;
    virtual void toggleAbort(bool yes) = 0;
    virtual ~PPOC() {};
};


class ProcessPresenter : public PMOC {
public:
    ProcessPresenter(PPOC* ppoc);
    virtual void displayProcessSummary(std::vector<std::string> processSummary);
    void displayState(State* s);
private:
    PPOC* ppoc;
};

#endif // PROCESSPRESENTER_H
