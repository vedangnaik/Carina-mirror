#include "StatesPresenter.h"

void StatesPresenter::displayState(const State& s) {
    if (s.transitions.at(Transition::Proceed) == "") {
        this->stpoc.allowProceed(false);
    } else {
        this->stpoc.allowProceed(true);
    }

    if (s.transitions.at(Transition::Abort) == "") {
        this->stpoc.allowAbort(false);
    } else {
        this->stpoc.allowAbort(true);
    }

    this->stpoc.displayState(s);
}

void StatesPresenter::displayStatesSummary(const std::vector<std::string> processSummary) {
    this->stpoc.displayProcessSummary(processSummary);
}
