#include "StatesManager.h"
#include <iostream>

StatesManager::StatesManager(map<const string, const State> states, SMIC& smic, AMIC& amic)
    : states{std::move(states)},  smic{smic}, amic{amic}
{}

void
StatesManager::transition(Transition t)
{
    const auto& sensorChecks = this->currentState->sensorChecks;

    std::vector<std::string> failures = {};
    for (auto p : sensorChecks.at(t)) {
        float value = this->smic.getSensorValue(p.first);
        SensorCheck& sc = p.second;
        if (std::abs(sc.a - value) + std::abs(sc.b - value) != std::abs(sc.a - sc.b)) {
            failures.push_back("Sensor ID '" + p.first + "' must be in range [" + std::to_string(sc.a) + ", " + std::to_string(sc.b) + "]");
        }
    }

    const auto& actuatorChecks = this->currentState->actuatorChecks;
    for (auto p : actuatorChecks.at(t)) {
        if (this->amic.getActuatorStatus(p.first) != p.second.status) {
            failures.push_back("Actautor ID '" + p.first + "' must be " + (p.second.status ? "on" : "off"));
        }
    }

    if (failures.size() == 0) {
        this->currentState = &this->states.at(this->currentState->transitions.at(t));
        this->stmoc->displayState(*this->currentState);
    } else {
        this->stmoc->displayFailedChecks(failures, t);
    }
}

void
StatesManager::startProcess()
{
    this->currentState = &this->states.at("start");
    this->stmoc->displayState(*this->currentState);

    std::vector<string> processSummary = {};
    const State* curr = this->currentState;
    processSummary.push_back(curr->description);
    while (curr->transitions.at(Transition::Proceed) != "") {
        curr = &this->states.at(curr->transitions.at(Transition::Proceed));
        processSummary.push_back(curr->description);
    }
    this->stmoc->displayStatesSummary(processSummary);
}

void
StatesManager::stopProcess()
{}
