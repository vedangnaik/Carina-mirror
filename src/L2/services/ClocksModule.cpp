#include "ClocksModule.h"

ClocksModule::ClocksModule()
{
    this->oneSTimer = new QTimer();
    this->HundredMsTimer = new QTimer();
}

void ClocksModule::start() {
    this->oneSTimer->start(1000);
    this->HundredMsTimer->start(100);
}

void ClocksModule::stop() {
    this->oneSTimer->stop();
    this->HundredMsTimer->stop();
}
