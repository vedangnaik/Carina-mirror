#include "StatesPresenter.h"

void StatesPresenter::displayState(const State& s) {
    s.transitions.at(Transition::Proceed) == "" ? this->stpoc->allowProceed(false) : this->stpoc->allowProceed(true);
    s.transitions.at(Transition::Abort) == "" ? this->stpoc->allowAbort(false) : this->stpoc->allowAbort(true);
    this->stpoc->displayState(s);
}

void StatesPresenter::displayStatesSummary(const std::vector<std::string> processSummary) {
    this->stpoc->displayProcessSummary(processSummary);
}

void
StatesPresenter::displayFailedChecks(const std::vector<std::string> failures, Transition t)
{
    this->stpoc->allowFailedChecksOverride(failures, t);
}

void
StatesPresenter::subscribe(StPOC* stpoc)
{
    this->stpoc = stpoc;
}

void
StatesPresenter::unsubscribe()
{
    this->stpoc = nullptr;
}
