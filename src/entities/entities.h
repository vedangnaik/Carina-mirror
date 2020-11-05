#ifndef ENTITIES_H
#define ENTITIES_H

#include <string>
#include <vector>
#include <map>
#include <ostream>


class Sensor {
public:
//    Sensor();
    std::string id;
    std::string name;
    std::vector<float> values;
};


class Actuator {
public:
//    Actuator();
    std::string id;
    std::string name;
    bool status;
};


class State {
public:
    std::string id;
    std::string name;
    std::string safetyRating;
    std::string description;
    std::vector<std::string> actions;

    std::string proceedState;
    std::string abortState;
};


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

#endif // ENTITIES_H
