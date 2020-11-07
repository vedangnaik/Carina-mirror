#ifndef SERVICES_H
#define SERVICES_H

#include <QTimer>

class ClocksModule
{
public:
    ClocksModule();
    QTimer* oneSTimer;
    QTimer* tenMsTimer;
};

#endif // SERVICES_H
