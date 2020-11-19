#include "StatesManager.h"
#include <iostream>


void StatesManager::transition(Transition t) {
    State* q = this->currentState;

    auto sensorChecks = q->getSensorChecks();
    for (auto p : sensorChecks.at(t)) {
        float value = this->smic.getSensorValue(p.first);
        SensorCheck sc = p.second;
        if (std::abs(sc.a - value) + std::abs(sc.b - value) != std::abs(sc.a - sc.b)) {
            return; // tell presenter what the error here is
        }
    }

    auto actuatorChecks = q->getActuatorChecks();
    for (auto p : actuatorChecks.at(t)) {
        if (this->amic.getActuatorStatus(p.first) != p.second.status) {
            return; // tell presenter what the error here is
        }
    }

    State* next = this->states.at(q->getTransitions().at(t));
    this->currentState = next;
    this->stmoc->displayState(*this->currentState);
}
