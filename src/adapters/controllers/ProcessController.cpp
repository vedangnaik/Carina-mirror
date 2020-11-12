#include "ProcessController.h"
#include "src/adapters/gateways/ProcessGateway.h"
#include <iostream>

ProcessController::ProcessController(PMIC* pmic) {
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
    this->pmic->createProcess(pdata.sensors, pdata.actuators, pdata.states);
    this->pmic->startProcess();
}
