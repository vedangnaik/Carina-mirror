#ifndef DAQPLACEHOLDER_H
#define DAQPLACEHOLDER_H

#include "src/L2/services/ClocksModule.h"
#include "src/L3/gateways/SensorValuesGateway.h"

class DAQPlaceholder : public QObject {
    Q_OBJECT
public:
    DAQPlaceholder(ClocksModule* cm, SVGIC* svgic);
    void startAcquisition();
    void stopAcquisition();
private:
    void acquire();
    ClocksModule* cm;
    SVGIC* svgic;
};

#endif // DAQPLACEHOLDER_H
