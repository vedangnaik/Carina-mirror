#ifndef PROCESSCONTROLLER_H
#define PROCESSCONTROLLER_H

#include <QObject>
#include <usecases.h>

class PCIC {
public:
    virtual void proceed() = 0;
    virtual void abort() = 0;
    virtual ~PCIC() {};
};

class ProcessController : public QObject, public PCIC
{
    Q_OBJECT
//    Q_INTERFACES(PCIC);
public:
    ProcessController(QObject *parent = nullptr);
    void proceed();
    void abort();

    PMIC* pmic;
};

#endif // PROCESSCONTROLLER_H
