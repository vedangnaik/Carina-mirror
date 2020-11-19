#include "ProcessManager.h"

ProcessManager::ProcessManager() {

}

void ProcessManager::createProcess(std::string filepath) {
    ProcessGateway pg(filepath);
    struct ProcessData pgdata = pg.parseProcessFile();
    this->cm = new ClocksModule();
    this->sm = new SensorsManager(pgdata.sensors);
    this->am = new ActuatorsManager(pgdata.actuators);
    this->stm = new StatesManager(pgdata.states, *this->sm, *this->am, *this->cm);

}

void ProcessManager::closeProcess() {

}

void ProcessManager::startProcess() {

}
