#include "ClocksModule.h"

ClocksModule::ClocksModule()
{
    this->oneSTimer = new QTimer();
    this->HundredMsTimer = new QTimer();

    this->oneSTimer->start(1000);
    this->HundredMsTimer->start(100);
}
