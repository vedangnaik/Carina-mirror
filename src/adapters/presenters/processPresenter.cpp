#include "presenters.h"

ProcessPresenter::ProcessPresenter(PPOC* ppoc) {
    this->ppoc = ppoc;
}

void ProcessPresenter::displayState(State* s) {
    if (s->proceedState == "") {
        this->ppoc->toggleProceed(false);
    } else {
        this->ppoc->toggleProceed(true);
    }

    if (s->abortState == "") {
        this->ppoc->toggleAbort(false);
    } else {
        this->ppoc->toggleAbort(true);
    }

    this->ppoc->displayState(s);
}

void ProcessPresenter::displayProcessSummary(std::vector<std::string> processSummary) {
    this->ppoc->displayProcessSummary(processSummary);
}
