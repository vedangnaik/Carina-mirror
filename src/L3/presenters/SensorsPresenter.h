#ifndef SENSORSPRESENTER_H
#define SENSORSPRESENTER_H

#include "src/L2/usecases/SensorsManager.h"
#include <stdexcept>
#include <map>
#include <iostream>

class SPOC {
public:
    virtual void displaySensorValue(const float value) = 0;
    virtual ~SPOC() {};
};

class SPIC {
public:
    virtual void subscribe(std::string id, SPOC* spoc) = 0;
    virtual ~SPIC() {};
};

class SensorsPresenter : public SMOC, public SPIC {
public:
    SensorsPresenter(ClocksModule& cm) : cm(cm) {};
    void displaySensorValue(const std::string id, const float value) override;
    void subscribe(std::string id, SPOC* spoc) override;
private:
    std::map<std::string, std::vector<SPOC*>> subscribers;
    ClocksModule& cm;
};

#endif // SENSORSPRESENTER_H
