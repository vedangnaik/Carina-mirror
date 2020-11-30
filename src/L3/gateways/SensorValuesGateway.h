#ifndef SENSORVALUESGATEWAY_H
#define SENSORVALUESGATEWAY_H

#include <QObject>
#include "src/L2/usecases/SensorsManager.h"

class SVGIC : public QObject {
    Q_OBJECT
public:
    virtual void updateValue(std::string id, float value) = 0;
    virtual std::vector<std::string> getSensorIDs() = 0;
    virtual ~SVGIC() {};
};


class SensorValuesGateway : public SVGIC {
public:
    SensorValuesGateway(SMIC& smic) : smic(smic) {};
    void updateValue(std::string id, float value);
    std::vector<std::string> getSensorIDs();
private:
    SMIC& smic;
};

#endif // SENSORVALUESGATEWAY_H
