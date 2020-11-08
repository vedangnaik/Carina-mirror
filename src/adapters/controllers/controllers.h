#ifndef PROCESSCONTROLLER_H
#define PROCESSCONTROLLER_H

#include <QObject>
#include "src/usecases/usecases.h"

class PCIC : public QObject
{
    Q_OBJECT
public:
    virtual void openProcess(std::string filePath) = 0;
    virtual void proceed() = 0;
    virtual void abort() = 0;
};

class ProcessController : public PCIC {
public:
    ProcessController(SMIC* smic, AMIC* amic, PMIC* pmic);
    void openProcess(std::string filePath);
    void proceed();
    void abort();
private:
    PMIC* pmic;
    SMIC* smic;
    AMIC* amic;
};

//====

class ACIC : public QObject {
    Q_OBJECT
public:
    virtual void actuate(std::string id) = 0;
};

class ActuatorsController : public ACIC {
public:
    ActuatorsController(AMIC* amic);
    void actuate(std::string id);
private:
   AMIC* amic;
};

#endif // PROCESSCONTROLLER_H
