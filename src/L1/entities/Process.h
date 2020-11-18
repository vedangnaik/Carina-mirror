#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <map>
#include "State.h"


class Process {
public:
    Process(std::map<std::string, State*> states, State* startState): states(states), currentState(startState) {};
    State* getStateById(std::string id) {
        return this->states[id];
    }
    State* getCurrentState() {
        return this->currentState;
    }
    void setCurrentState(State* s) {
        this->currentState = s;
    };
private:
    std::map<std::string, State*> states;
    State* currentState;
};

#endif // PROCESS_H
