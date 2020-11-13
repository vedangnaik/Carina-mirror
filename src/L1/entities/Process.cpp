#include "Process.h"

Process::Process(std::map<std::string, State*> Q, State* startState) {
    this->Q = Q;
    this->currentState = startState;
}

State* Process::getStateById(std::string id) {
    return this->Q[id];
}

State* Process::getCurrentState() {
    return this->currentState;
}

void Process::setCurrentState(State* s) {
    this->currentState = s;
}
