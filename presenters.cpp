#include "presenters.h"

void ProcessPresenter::displayState(State* s) {
    bool proceed = true;
    bool abort = false;
    if (s->proceedState == "") { proceed = false; }
    if (s->abortState == "") { abort = false; }
    this->ppoc->displayState(s);
}
