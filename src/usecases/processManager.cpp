#include "usecases.h"
#include <iostream>

void ProcessManager::setOutputContract(PMOC* pmoc) {
    this->pmoc = pmoc;
}

void ProcessManager::transition(Transition t) {
    State* q = this->p->getCurrentState();

    try {
        State* next = this->p->getStateById(q->transitions.at(t));
        this->p->setCurrentState(next);
        this->pmoc->displayState(this->p->getCurrentState());
    }  catch (std::out_of_range& e) {
        // shit
    }
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

void ProcessManager::startProcess() {
    std::vector<std::string> processSummary = {};

    State* curr = this->p->getCurrentState();
    processSummary.push_back(curr->description);
    while (curr->transitions[Transition::Proceed] != "") {
        curr = this->p->getStateById(curr->transitions[Transition::Proceed]);
        processSummary.push_back(curr->description);
    }
    this->pmoc->displayProcessSummary(processSummary);

    this->pmoc->displayState(this->p->getCurrentState());
}
