#include "usecases.h"

#include <iostream>

void ProcessManager::transition(std::string transition) {
    State* q = this->p->getCurrentState();

    State* next;
    if (transition == "proceed") {
        next = this->p->getStateById(q->proceedState);
    } else if (transition == "abort") {
        next = this->p->getStateById(q->abortState);
    }

    this->p->setCurrentState(next);
    this->pmoc->displayState(this->p->getCurrentState());
}

void ProcessManager::createProcess(std::vector<State*> states) {
    try {
        std::map<std::string, State*> Q;
        for (State* s : states) {
            Q[s->id] = s;
        }
        State* startState = Q.at("start");
        this->p = new Process(Q, startState);
    }  catch (std::out_of_range& e) {
        // handle the error here by asking the presenter to display it.
        std::cout << "out of range error" << std::endl;
    }

}
