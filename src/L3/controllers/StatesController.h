#pragma once

#include <QObject>
#include "StatesManager.h"

class StCIC : public QObject {
    Q_OBJECT
public:
    virtual void proceed(bool override = false) = 0;
    virtual void abort(bool override = false) = 0;
    virtual ~StCIC() {};
};

class StatesController : public StCIC {
public:
    StatesController(StMIC& stmic) : stmic(stmic) {};
    void proceed(bool override = false) override;
    void abort(bool override = false) override;
private:
    StMIC& stmic;
};
