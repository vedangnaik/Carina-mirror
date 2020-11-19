#ifndef STATESCONTROLLER_H
#define STATESCONTROLLER_H

#include <QObject>
#include "src/L2/usecases/StatesManager.h"

class StCIC : public QObject {
    Q_OBJECT
public:
    virtual void proceed() = 0;
    virtual void abort() = 0;
};

class StatesController : public StCIC {
public:
    StatesController(StMIC& stmic) : stmic(stmic) {};
    void proceed();
    void abort();
private:
    StMIC& stmic;
};

#endif // STATESCONTROLLER_H
