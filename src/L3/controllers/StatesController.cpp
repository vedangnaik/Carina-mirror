#include "StatesController.h"

void StatesController::proceed() {
    this->stmic->transition(Transition::Proceed);
}

void StatesController::abort() {
    this->stmic->transition(Transition::Abort);
}
