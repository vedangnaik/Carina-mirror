#ifndef PROCESSPRESENTER_H
#define PROCESSPRESENTER_H

#include "src/L2/usecases/ProcessManager.h"
#include "src/L1/entities/State.h"

struct StateDisplayInfo {
    std::string id;
    std::string name;
    std::string safetyRating;
    std::string description;

    std::vector<std::string> actionsOrder;
    std::map<std::string, std::vector<SensorOption>> sensorOptions;
    std::map<std::string, std::vector<ActuatorOption>> actuatorOptions;

    std::string proceedState;
    std::string abortState;
};

class PPOC {
public:
    virtual void displayProcessSummary(std::vector<std::string> processSummary) = 0;
    virtual void displayState(StateDisplayInfo sdi) = 0;
    virtual void allowProceed(bool permission) = 0;
    virtual void allowAbort(bool permission) = 0;
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
