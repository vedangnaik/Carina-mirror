#ifndef PROCESSPRESENTER_H
#define PROCESSPRESENTER_H

#include "src/usecases/usecases.h"

class PPOC {
public:
    virtual void displayProcessSummary(std::vector<std::string> processSummary) = 0;
    virtual void displayState(
            std::string name,
            std::string description,
            std::string abortState,
            std::map<int, Sensor*> sensorPos,
            std::map<int, Actuator*> actuatorPos
        ) = 0;
    virtual void allowProceed(bool permission) = 0;
    virtual void allowAbort(bool permission) = 0;
    virtual ~PPOC() {};
};


class ProcessPresenter : public PMOC {
public:
    ProcessPresenter(SMIC* smic, AMIC* amic, PPOC* ppoc);
    virtual void displayProcessSummary(std::vector<std::string> processSummary);
    void displayState(State* s);
private:
    SMIC* smic;
    AMIC* amic;
    PPOC* ppoc;
};

#endif // PROCESSPRESENTER_H
