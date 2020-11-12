#include "ClocksModule.h"

ClocksModule::ClocksModule()
{
    this->oneSTimer = new QTimer();
    this->tenMsTimer = new QTimer();

    this->oneSTimer->start(1000);
    // this->tenMsTimer->start(10);
}
