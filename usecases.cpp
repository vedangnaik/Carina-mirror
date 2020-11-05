#include "usecases.h"

#include <iostream>

void ProcessManager::transition(std::string transition) {
    this->p->currentState = this->p->currentState->transitions[transition];
    this->pmoc->displayState(this->p->currentState);
}

void ProcessManager::createProcess(std::vector<StateDTO> Q) {
    this->p = new Process();
    for (StateDTO s : Q) {
        State* state = new State();
        state->name = s.name;
        state->safetyRating = s.safetyRating;
        state->description = s.description;
        state->actions = {};
        for (std::string actionId: s.actions) {
            // use Sensor/ActuatorManagers here to load in the appropriate Actions
            // state->actions.push_back(sm[actionID])
        }
        this->p->Q[s.id] = state;
    }

    for (StateDTO s : Q) {
        if (s.proceedState != "") {
            this->p->Q[s.id]->transitions["proceed"] = this->p->Q[s.proceedState];
        }
        if (s.abortState != "") {
            this->p->Q[s.id]->transitions["abort"] = this->p->Q[s.abortState];
        }
    }

    this->p->currentState = this->p->Q["start"];
}


void SensorsManager::createSensor(std::string id, std::string name) {
    Sensor* s = new Sensor(name);
    this->listOfSensors[id] = s;
}
