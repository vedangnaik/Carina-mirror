#include "StatesManager.h"
#include <iostream>

StatesManager::StatesManager(AMIC* amic, SMIC* smic, ClocksModule* cm) {
    this->amic = amic;
    this->smic = smic;
    this->cm = cm;
}


void StatesManager::setOutputContract(StMOC* pmoc) {
    this->pmoc = pmoc;
}


void StatesManager::transition(Transition t) {
    State* q = this->currentState;

    for (auto p : q->sensorChecks[t]) {
        float value = this->smic->getSensorValue(p.first);
        SensorCheck sc = p.second;
        if (std::abs(sc.a - value) + std::abs(sc.b - value) != std::abs(sc.a - sc.b)) {
            return; // tell presenter what the error here is
        }
    }

    for (auto p : q->actuatorChecks[t]) {
        if (this->amic->getActuatorStatus(p.first) != p.second.status) {
            return; // tell presenter what the error here is
        }
    }

    State* next = this->states.at(q->transitions[t]);
    this->currentState = next;
    this->pmoc->displayState(*this->currentState);
}


void StatesManager::createProcess(std::map<std::string, Sensor*> sensors, std::map<std::string, Actuator*> actuators, std::map<std::string, State*> states) {
    try {
        this->smic->addSensors(sensors);
        this->amic->addActuators(actuators);
        this->states = states;
        this->currentState = states.at("start");
    }  catch (std::out_of_range& e) {
        // handle the error here by asking the presenter to display it.
    }
}


void StatesManager::startProcess() {
    std::vector<std::string> processSummary = {};

    State* curr = this->currentState;
    processSummary.push_back(curr->description);
    while (curr->transitions[Transition::Proceed] != "") {
        curr = this->states.at(curr->transitions[Transition::Proceed]);
        processSummary.push_back(curr->description);
    }
    this->pmoc->displayStatesSummary(processSummary);

    this->pmoc->displayState(*this->currentState);
    this->cm->start();
}
