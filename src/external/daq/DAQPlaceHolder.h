#ifndef DAQPLACEHOLDER_H
#define DAQPLACEHOLDER_H

#include "src/external/services/ClocksModule.h"
#include "src/adapters/controllers/SensorsController.h"

class DAQPlaceholder
{
public:
    DAQPlaceholder(ClocksModule* cm, SCIC* scic);
    void startAcquisition();
private:
    ClocksModule* cm;
    SCIC* scic;
};

#endif // DAQPLACEHOLDER_H
