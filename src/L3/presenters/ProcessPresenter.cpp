#include "ProcessPresenter.h"

ProcessPresenter::ProcessPresenter(PPOC* ppoc) {
    this->ppoc = ppoc;
}

void ProcessPresenter::displayState(const State& s) {
    StateDisplayInfo sdi;
    sdi.id = s.id;
    sdi.name = s.name;
    sdi.safetyRating = s.safetyRating;
    sdi.description = s.description;
    sdi.actionsOrder = s.actionsOrder;
    sdi.sensorOptions = s.sensorOptions;
    sdi.actuatorOptions = s.actuatorOptions;
    sdi.proceedState = s.transitions.at(Transition::Proceed);
    sdi.abortState = s.transitions.at(Transition::Abort);

    if (sdi.proceedState == "") {
        this->ppoc->allowProceed(false);
    } else {
        this->ppoc->allowProceed(true);
    }

    if (sdi.abortState == "") {
        this->ppoc->allowAbort(false);
    } else {
        this->ppoc->allowAbort(true);
    }

    this->ppoc->displayState(sdi);
}

void ProcessPresenter::displayProcessSummary(std::vector<std::string> processSummary) {
    this->ppoc->displayProcessSummary(processSummary);
}
