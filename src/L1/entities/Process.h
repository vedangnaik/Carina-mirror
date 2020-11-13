#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <map>
#include "State.h"


class Process {
public:
    Process(std::map<std::string, State*> Q, State* startState);
    State* getStateById(std::string id);
    State* getCurrentState();
    void setCurrentState(State* s);
private:
    std::map<std::string, State*> Q;
    State* currentState;
};

#endif // PROCESS_H
