#include "StatesPresenter.h"

StatesPresenter::StatesPresenter(SPOC* ppoc) {
    this->spoc = ppoc;
}

void StatesPresenter::displayState(const State& s) {
    if (s.transitions.at(Transition::Proceed) == "") {
        this->spoc->allowProceed(false);
    } else {
        this->spoc->allowProceed(true);
    }

    if (s.transitions.at(Transition::Abort) == "") {
        this->spoc->allowAbort(false);
    } else {
        this->spoc->allowAbort(true);
    }

    this->spoc->displayState(s);
}

void StatesPresenter::displayStatesSummary(std::vector<std::string> processSummary) {
    this->spoc->displayProcessSummary(processSummary);
}
