#ifndef ENTITIES_H
#define ENTITIES_H

#include <string>
#include <vector>
#include <map>
#include <ostream>


class Sensor {
public:
    Sensor(std::string id, std::string name);
    std::string id;
    std::string name;
    std::vector<float> values;
};


class Actuator {
public:
    Actuator(std::string id, std::string name);
    std::string id;
    std::string name;
    bool status;
};
enum ActuatorOption {
    Timed,
    Automatic
};
enum ActuatorCheck {
    Close = 0,
    Open = 1
};


enum Transition {
    Proceed,
    Abort
};
class State {
public:
    std::string id;
    std::string name;
    std::string safetyRating;
    std::string description;
    std::vector<std::pair<std::string, std::vector<unsigned int>>> actionOptions;
    std::map<Transition, std::map<std::string, std::vector<unsigned int>>> actionsChecks;
    std::map<Transition, std::string> transitions;
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
