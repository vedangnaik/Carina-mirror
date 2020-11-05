#include "presenters.h"

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
