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
    State(std::string name, std::string safetyRating,
          std::string description, std::vector<Action*> actions);

    std::string name;
    std::string safetyRating;
    std::string description;
    std::vector<Action*> actions;
    std::map<std::string, State*> transitions;
};


class Process {
public:
    Process(
        std::vector<State*> Q,
        std::map<std::pair<State*, std::string>, State*> delta,
        State* s,
        State* F
    );

    std::vector<State*> Q;
    std::vector<std::string> Sigma = {"proceed", "abort"};
    std::map<std::pair<State*, std::string>, State*> delta;
    State* s;
    State* F;
    State* currentState;
};

#endif // ENTITIES_H
