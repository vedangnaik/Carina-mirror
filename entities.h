#ifndef ENTITIES_H
#define ENTITIES_H

#include <string>
#include <vector>
#include <map>
#include <ostream>


class Action {
public:
    std::string name;
    virtual ~Action() {};
};


class Sensor: public Action {
public:
    Sensor(std::string name);
    std::vector<float> values;
};


class Actuator: public Action {
public:
    Actuator(std::string name, bool status);
    bool status;
};


class State {
public:
    std::string name;
    std::string safetyRating;
    std::string description;
    std::vector<Action*> actions;
    std::map<std::string, State*> transitions;
};


class Process {
public:
    std::vector<State*> Q;
    State* s;
    State* currentState;
};

#endif // ENTITIES_H
