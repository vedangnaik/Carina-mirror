#include "ProcessController.h"

struct ProcessData ProcessController::openProcess(std::string filePath) {
    ProcessGateway* pg = new ProcessGateway();
    return pg->parseProcessFile(filePath);
}

void ProcessController::startProcess() {

}

void ProcessController::closeProcess() {

}
