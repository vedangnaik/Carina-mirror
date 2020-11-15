#ifndef PROCESSCONTROLLER_H
#define PROCESSCONTROLLER_H

#include <QObject>
#include "src/L2/usecases/ActuatorsManager.h"
#include "src/L2/usecases/SensorsManager.h"
#include "src/L2/usecases/ProcessManager.h"
#include "src/L3/gateways/ProcessGateway.h"
#include <iostream>

class PCIC : public QObject {
    Q_OBJECT
public:
    virtual void openProcess(std::string filePath) = 0;
    virtual void proceed() = 0;
    virtual void abort() = 0;
};

class ProcessController : public PCIC {
public:
    ProcessController(PMIC* pmic);
    void openProcess(std::string filePath);
    void proceed();
    void abort();
private:
    PMIC* pmic;
};

#endif // PROCESSCONTROLLER_H
