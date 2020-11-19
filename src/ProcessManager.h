#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

// L1
// L2
#include "src/L2/usecases/SensorsManager.h"
#include "src/L2/usecases/ActuatorsManager.h"
#include "src/L2/usecases/StatesManager.h"
#include "src/L2/services/ClocksModule.h"
// L3
//#include "src/L3/controllers/ActuatorsController.h"
//#include "src/L3/controllers/ProcessController.h"
#include "src/L3/gateways/ProcessGateway.h"
//#include "src/L3/gateways/SensorValuesGateway.h"
//#include "src/L3/presenters/StatesPresenter.h"
// L4
//#include "src/L4/daq/DAQPlaceHolder.h"
//#include "src/L4/ui/ProcessUIHandler.h"
//#include "src/L4/ui/StateUIHandler.h"

#include <string>

class PMIC {
public:
    virtual void createProcess(std::string filepath) = 0;
    virtual void startProcess() = 0;
    virtual void closeProcess() = 0;
};

class ProcessManager : public PMIC {
public:
    ProcessManager();
    void createProcess(std::string filepath);
    void startProcess();
    void closeProcess();
private:
    SensorsManager* sm;
    ActuatorsManager* am;
    StatesManager* stm;
    ClocksModule* cm;
};

#endif // PROCESSMANAGER_H
