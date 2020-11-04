#include "usecases.h"

#include <iostream>

ProcessManager::ProcessManager(ProcessManagerOutputContract* icuop) {
    this->icuop = icuop;
}

void ProcessManager::transition(std::string alphabet) {
    State* s = this->p->currentState;
    this->p->currentState = this->p->delta[std::make_pair(s, alphabet)];
}


void SensorsManager::createSensor(std::string id, std::string name) {
    Sensor* s = new Sensor(name);
    this->listOfSensors[id] = s;
}
