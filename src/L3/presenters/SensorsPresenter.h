#ifndef SENSORSPRESENTER_H
#define SENSORSPRESENTER_H

#include "src/L2/usecases/SensorsManager.h"

class SPOC {
public:
    virtual void displaySensorValue(const std::string id, const float value) = 0;
};

class SensorsPresenter : public SMOC {
public:
    SensorsPresenter(std::vector<SPOC*> subscribers) : subscribers(subscribers) {};
    void displaySensorValue(const std::string id, const float value);
    void subscribe(SPOC* spoc);
private:
    std::vector<SPOC*> subscribers;
};

#endif // SENSORSPRESENTER_H
