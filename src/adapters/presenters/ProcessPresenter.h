#ifndef PROCESSPRESENTER_H
#define PROCESSPRESENTER_H

#include "src/usecases/SensorsManager.h"
#include "src/usecases/ActuatorsManager.h"
#include "src/entities/State.h"
#include "src/usecases/ProcessManager.h"

struct StateDisplayInfo {
    std::string id;
    std::string name;
    std::string safetyRating;
    std::string description;
    std::vector<std::pair<std::string, std::vector<unsigned int>>> actions;
    std::map<std::string, std::string> whatIsActions;
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
    ProcessPresenter(SMIC* smic, AMIC* amic, PPOC* ppoc);
    virtual void displayProcessSummary(std::vector<std::string> processSummary);
    void displayState(State* s);
private:
    SMIC* smic;
    AMIC* amic;
    PPOC* ppoc;
};

#endif // PROCESSPRESENTER_H
