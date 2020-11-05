#ifndef PROCESSCONTROLLER_H
#define PROCESSCONTROLLER_H

#include <QObject>
#include <usecases.h>

class PCIC : public QObject
{
    Q_OBJECT
public:
    PCIC() {}
    virtual ~PCIC() {}
    virtual void proceed() = 0;
    virtual void abort() = 0;
};

//class PCIC {
//public:
//    virtual void proceed() = 0;
//    virtual void abort() = 0;
//    virtual ~PCIC() {};
//};

class ProcessController : public PCIC {
public:
//    ProcessController();
    void proceed();
    void abort();

    PMIC* pmic;
};

#endif // PROCESSCONTROLLER_H
