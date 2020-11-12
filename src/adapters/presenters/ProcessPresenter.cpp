#include "ProcessPresenter.h"

ProcessPresenter::ProcessPresenter(PPOC* ppoc) {
    this->ppoc = ppoc;
}

void ProcessPresenter::displayState(State* s) {
    StateDisplayInfo sdi;
    sdi.id = s->id;
    sdi.name = s->name;
    sdi.safetyRating = s->safetyRating;
    sdi.description = s->description;
    sdi.actionsOrder = s->actionsOrder;
    sdi.sensorOptions = s->sensorOptions;
    sdi.actuatorOptions = s->actuatorOptions;
    sdi.proceedState = s->transitions[Transition::Proceed];
    sdi.abortState = s->transitions[Transition::Abort];

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

//    std::map<std::string, std::string> whatIsActions = {};
//    for (auto a : s->actionOptions) {
//        std::string id = a.first;
//        if (this->amic->getActuatorStatus(id) != nullptr) {
//            whatIsActions[id] = "actuator";
//        } else if (this->smic->getSensorValue(id) != nullptr) {
//            whatIsActions[id] = "sensor";
//        } else {
//            // well shit, we shouldn't really be here...
//        }
//    }
//    sdi.whatIsActions = whatIsActions;

    this->ppoc->displayState(sdi);
}

void ProcessPresenter::displayProcessSummary(std::vector<std::string> processSummary) {
    this->ppoc->displayProcessSummary(processSummary);
}
