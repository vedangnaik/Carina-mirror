#include "presenters.h"

ProcessPresenter::ProcessPresenter(SMIC* smic, AMIC* amic, PPOC* ppoc) {
    this->smic = smic;
    this->amic = amic;
    this->ppoc = ppoc;
}

void ProcessPresenter::displayState(State* s) {
    if (s->proceedState == "") {
        this->ppoc->allowProceed(false);
    } else {
        this->ppoc->allowProceed(true);
    }

    if (s->abortState == "") {
        this->ppoc->allowAbort(false);
    } else {
        this->ppoc->allowAbort(true);
    }

    std::map<std::string, Sensor*> sensors;
    std::map<std::string, Actuator*> actuators;
    for (auto a : s->actions) {
        std::string id = a.first;
        if (this->amic->findActuator(id) != nullptr) {
            actuators[id] = this->amic->findActuator(id);
        } else if (this->smic->findSensor(id) != nullptr) {
            sensors[id] = this->smic->findSensor(id);
        } else {
            // well shit, we shouldn't really be here...
        }
    }

    this->ppoc->displayState(s, actuators, sensors);
}

void ProcessPresenter::displayProcessSummary(std::vector<std::string> processSummary) {
    this->ppoc->displayProcessSummary(processSummary);
}
