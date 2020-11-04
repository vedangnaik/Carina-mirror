#include "entities.h"


Sensor::Sensor(std::string name) {
    this->name = name;
    this->values = {};
}


Actuator::Actuator(std::string name, bool status) {
    this->name = name;
    this->status = status;
}


State::State(std::string name, std::string safetyRating,
      std::string description, std::vector<Action*> actions) {
    this->name = name;
    this->safetyRating = safetyRating;
    this->description = description;
    this->actions = actions;
}


Process::Process(
    std::vector<State*> Q,
    std::map<std::pair<State*, std::string>, State*> delta,
    State* s,
    State* F
) {
    this->Q = Q;
    this->delta = delta;
    this->s = s;
    this->F = F;
    this->currentState = s;
}
