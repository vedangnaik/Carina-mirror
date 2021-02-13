#include "StatesController.h"

void StatesController::proceed(bool override) {
    this->stmic.transition(Transition::Proceed, override);
}

void StatesController::abort(bool override) {
    this->stmic.transition(Transition::Abort, override);
}
