#include "ProcessController.h"
#include "src/adapters/gateways/ProcessGateway.h"
#include <iostream>

ProcessController::ProcessController(SMIC* smic, AMIC* amic, PMIC* pmic) {
    this->smic = smic;
    this->amic = amic;
    this->pmic = pmic;
}

void ProcessController::proceed() {
    this->pmic->transition(Transition::Proceed);
}

void ProcessController::abort() {
    this->pmic->transition(Transition::Abort);
}

void ProcessController::openProcess(std::string filePath) {
    ProcessGateway* pg = new ProcessGateway();
    struct ProcessData pdata = pg->parseProcessFile(filePath);
    this->smic->addSensors(pdata.sensors);
    this->amic->addActuators(pdata.actuators);
    this->pmic->createProcess(pdata.states);
    this->pmic->startProcess();
}
