#pragma once

#include "AbstractDAQDeviceHandler.h"
#include "easylogging++.h"
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>

class SerialPortHandler : public AbstractDAQDeviceHandler {
public:
    SerialPortHandler(std::string id, std::string serialportPath, unsigned int numChannels);
    void startAcquisition() override;
    std::vector<double> getLatestData() override;
    void stopAcquisition() override;
private:
    const std::string serialportPath;
    std::ifstream serialport;
};

