#include "controllers.h"
#include "gateways.h"
#include <iostream>

void ProcessController::proceed() {
    this->pmic->transition("proceed");
}

void ProcessController::abort() {
    this->pmic->transition("abort");
}

void ProcessController::openProcess(std::string filePath) {
    std::cout << filePath << std::endl;
    ProcessGateway* pg = new ProcessGateway();
    struct ProcessData pdata = pg->parseProcessFile(filePath);
    this->pmic->createProcess(pdata.states);
    // Use other pdata fields to make sensors and actuators here
}
