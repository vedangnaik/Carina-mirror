#include "ProcessPresenter.h"

ProcessPresenter::ProcessPresenter(PPOC* ppoc) {
    this->ppoc = ppoc;
}

void ProcessPresenter::displayState(const State& s) {
    if (s.transitions.at(Transition::Proceed) == "") {
        this->ppoc->allowProceed(false);
    } else {
        this->ppoc->allowProceed(true);
    }

    if (s.transitions.at(Transition::Abort) == "") {
        this->ppoc->allowAbort(false);
    } else {
        this->ppoc->allowAbort(true);
    }

    this->ppoc->displayState(s);
}

void ProcessPresenter::displayStatesSummary(std::vector<std::string> processSummary) {
    this->ppoc->displayProcessSummary(processSummary);
}
