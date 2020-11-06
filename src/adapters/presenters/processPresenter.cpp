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

    std::map<int, Sensor*> sensorPos;
    std::map<int, Actuator*> actuatorPos;
    for (unsigned long i = 0; i < s->actions.size(); i++) {
        std::string key = s->actions[i];
        if (this->amic->findActuator(key) != nullptr) {
            actuatorPos[i] = this->amic->findActuator(key);
        } else if (this->smic->findSensor(key) != nullptr) {
            sensorPos[i] = this->smic->findSensor(key);
        } else {
            // well shit, we shouldn't really be here...
        }
    }

    this->ppoc->displayState(s->name, s->description, s->abortState, sensorPos, actuatorPos);
}

void ProcessPresenter::displayProcessSummary(std::vector<std::string> processSummary) {
    this->ppoc->displayProcessSummary(processSummary);
}
