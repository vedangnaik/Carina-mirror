#include "ProcessManager.h"
#include <iostream>

ProcessManager::ProcessManager(AMIC* amic, SMIC* smic, ClocksModule* cm) {
    this->amic = amic;
    this->smic = smic;
    this->cm = cm;
}


void ProcessManager::setOutputContract(PMOC* pmoc) {
    this->pmoc = pmoc;
}


void ProcessManager::transition(Transition t) {
    State* q = this->p->getCurrentState();

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

    State* next = this->p->getStateById(q->transitions[t]);
    this->p->setCurrentState(next);
    this->pmoc->displayState(this->p->getCurrentState());
}


void ProcessManager::createProcess(std::map<std::string, Sensor*> sensors, std::map<std::string, Actuator*> actuators, std::map<std::string, State*> states) {
    try {
        this->smic->addSensors(sensors);
        this->amic->addActuators(actuators);
        this->p = new Process(states, states.at("start"));
    }  catch (std::out_of_range& e) {
        // handle the error here by asking the presenter to display it.
    }
}


void ProcessManager::startProcess() {
    std::vector<std::string> processSummary = {};

    State* curr = this->p->getCurrentState();
    processSummary.push_back(curr->description);
    while (curr->transitions[Transition::Proceed] != "") {
        curr = this->p->getStateById(curr->transitions[Transition::Proceed]);
        processSummary.push_back(curr->description);
    }
    this->pmoc->displayProcessSummary(processSummary);

    this->pmoc->displayState(this->p->getCurrentState());
    this->cm->start();
}
