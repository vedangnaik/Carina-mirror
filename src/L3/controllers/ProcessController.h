#ifndef PROCESSCONTROLLER_H
#define PROCESSCONTROLLER_H

#include <QObject>
#include "src/L2/usecases/ActuatorsManager.h"
#include "src/L2/usecases/SensorsManager.h"
#include "src/L2/usecases/StatesManager.h"
#include "src/L3/gateways/ProcessGateway.h"
#include <iostream>

class PCIC : public QObject {
    Q_OBJECT
public:
    virtual struct ProcessData openProcess(std::string filePath) = 0;
    virtual void startProcess() = 0;
    virtual void closeProcess() = 0;
};

class ProcessController : public PCIC {
public:
    struct ProcessData openProcess(std::string filePath);
    void startProcess();
    void closeProcess();
};

#endif // PROCESSCONTROLLER_H
