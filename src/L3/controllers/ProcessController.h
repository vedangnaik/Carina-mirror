#ifndef PROCESSCONTROLLER_H
#define PROCESSCONTROLLER_H

#include <QObject>
#include "src/L2/usecases/ProcessManager.h"
#include "src/L3/gateways/ProcessGateway.h"
#include <iostream>

class PCIC : public QObject {
    Q_OBJECT
public:
    virtual void openProcess(std::string filePath) = 0;
    virtual void startProcess() = 0;
    virtual void closeProcess() = 0;
};

class ProcessController : public PCIC {
public:
    ProcessController(PMIC& pmic) : pmic(pmic) {};
    void openProcess(std::string filePath);
    void startProcess();
    void closeProcess();
private:
    PMIC& pmic;
};

#endif // PROCESSCONTROLLER_H
