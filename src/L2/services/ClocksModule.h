#ifndef SERVICES_H
#define SERVICES_H

#include <QTimer>

class ClocksModule
{
public:
    ClocksModule();
    void start();
    void stop();
    QTimer* oneSTimer;
    QTimer* HundredMsTimer;
};

#endif // SERVICES_H
