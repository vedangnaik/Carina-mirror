#pragma once

#include "IDAQDeviceHandler.h"
#include "easylogging++.h"
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>

class SerialPortHandler : public IDAQDeviceHandler {
public:
    SerialPortHandler(std::string id, std::string serialportPath, unsigned int numChannels);
    void startAcquisition() override;
    std::vector<double> getLatestData() override;
    void stopAcquisition() override;
private:
    std::string id;
    std::string serialportPath;
    unsigned int numChannels;
    std::ifstream serialport;
};

