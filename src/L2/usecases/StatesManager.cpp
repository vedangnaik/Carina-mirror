#include "StatesManager.h"
#include <iostream>

StatesManager::StatesManager(map<const string, const State> states, SMIC& smic, AMIC& amic)
    : states{std::move(states)},  smic{smic}, amic{amic}
{
    // start state not found
    if (this->states.find("start") == this->states.end()){
        throw NoStartStateError();
    }
}

void
StatesManager::transition(Transition t)
{
    const auto& sensorChecks = this->currentState->sensorChecks;
    for (auto p : sensorChecks.at(t)) {
        float value = this->smic.getSensorValue(p.first);
        SensorCheck& sc = p.second;
        if (std::abs(sc.a - value) + std::abs(sc.b - value) != std::abs(sc.a - sc.b)) {
            return; // tell presenter what the error here is
        }
    }

    const auto& actuatorChecks = this->currentState->actuatorChecks;
    for (auto p : actuatorChecks.at(t)) {
        if (this->amic.getActuatorStatus(p.first) != p.second.status) {
            return; // tell presenter what the error here is
        }
    }

    this->currentState = &this->states.at(this->currentState->transitions.at(t));

    // Make sure that this display output is not null.
    if (this->stmoc == nullptr) {
        std::string transitionName;
        if (t) transitionName = ", Abort";
        else transitionName = ", Proceed";

        LOG(ERROR) << "StatesManager::transition(" << this->currentState->name << transitionName << "): "
                        "this->stmoc is nullptr, not displaying state.";
    } else {
        this->stmoc->displayState(*this->currentState);
    }
}

void
StatesManager::startProcess()
{
    try {
        this->currentState = &this->states.at("start");
    } catch (std::out_of_range& e) {
        LOG(FATAL) << "StatesManager::startProcess: \"start\" state not found. Exception: " << e.what();
        std::terminate();
    }
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
