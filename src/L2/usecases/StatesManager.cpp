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
      
        // Make sure that this display output is not null.
        if (this->stmoc == nullptr) {          
            LOG(FATAL) << this.currentState->name << ": Transition '" << (t == Transition::Proceed ? "Proceed" : "Abort") << "' display not possible as this->stmoc is nullptr.";
        }
      
        this->stmoc->displayState(*this->currentState);
    } else {
        this->stmoc->displayFailedChecks(failures, t);
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
