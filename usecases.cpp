#include "usecases.h"

#include <iostream>

ProcessManager::ProcessManager(ProcessManagerOutputContract* icuop) {
    this->icuop = icuop;
}

void ProcessManager::transition(std::string alphabet) {

}

void ProcessManager::createProcess(std::vector<StateDTO> Q) {

}


void SensorsManager::createSensor(std::string id, std::string name) {
    Sensor* s = new Sensor(name);
    this->listOfSensors[id] = s;
}
