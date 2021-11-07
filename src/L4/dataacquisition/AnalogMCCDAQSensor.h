#ifdef ULDAQ_AVAILABLE
#pragma once

#include "Sensor.h"
#include "easylogging++.h"
#include "MCCDAQHandler.h"
#include <uldaq.h>
#include <cstdlib>
#include <map>
#include <vector>
#include <memory>
#include <iostream>
#include <cmath>

class AnalogMCCDAQSensor : public Sensor {
public:
    AnalogMCCDAQSensor(const std::string& deviceID,
        const std::vector<std::pair<double, double>>& calibrationPoints,
        std::shared_ptr<MCCDAQHandler> daq,
        unsigned int channelConnectedTo);
    void startAcquisition() override;
    double getLatestData() override;
    void stopAcquisition() override;
private:
    const unsigned int channelConnectedTo;
    std::shared_ptr<MCCDAQHandler> daq;
};

#endif
