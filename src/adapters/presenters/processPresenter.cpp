#include "presenters.h"

ProcessPresenter::ProcessPresenter(SMIC* smic, AMIC* amic, PPOC* ppoc) {
    this->smic = smic;
    this->amic = amic;
    this->ppoc = ppoc;
}

void ProcessPresenter::displayState(State* s) {
    StateDisplayInfo sdi;
    sdi.id = s->id;
    sdi.name = s->name;
    sdi.safetyRating = s->safetyRating;
    sdi.description = s->description;
    sdi.actions = s->actionOptions;
    sdi.abortState = s->transitions[Transition::Abort];

    if (s->transitions[Transition::Proceed] == "") {
        this->ppoc->allowProceed(false);
    } else {
        this->ppoc->allowProceed(true);
    }

    if (s->transitions[Transition::Abort] == "") {
        this->ppoc->allowAbort(false);
    } else {
        this->ppoc->allowAbort(true);
    }

    std::map<std::string, std::string> whatIsActions = {};
    for (auto a : s->actionOptions) {
        std::string id = a.first;
        if (this->amic->findActuator(id) != nullptr) {
            whatIsActions[id] = "actuator";
        } else if (this->smic->findSensor(id) != nullptr) {
            whatIsActions[id] = "sensor";
        } else {
            // well shit, we shouldn't really be here...
        }
    }
    sdi.whatIsActions = whatIsActions;

    this->ppoc->displayState(sdi);
}

void ProcessPresenter::displayProcessSummary(std::vector<std::string> processSummary) {
    this->ppoc->displayProcessSummary(processSummary);
}
