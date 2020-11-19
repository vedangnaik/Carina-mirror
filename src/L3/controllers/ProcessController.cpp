#include "ProcessController.h"

void ProcessController::openProcess(std::string filePath) {
    ProcessGateway* pg = new ProcessGateway();
    struct ProcessData pgdata = pg->parseProcessFile(filePath);
    this->pmic.openProcess(pgdata.sensors, pgdata.actuators, pgdata.states);
}

void ProcessController::startProcess() {

}

void ProcessController::closeProcess() {

}
