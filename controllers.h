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
    virtual void openProcess(std::string filePath) = 0;
    virtual void proceed() = 0;
    virtual void abort() = 0;
};

class ProcessController : public PCIC {
public:
    void openProcess(std::string filePath);
    void proceed();
    void abort();

    PMIC* pmic;
};

#endif // PROCESSCONTROLLER_H
