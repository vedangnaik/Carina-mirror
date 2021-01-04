#pragma once

#include "DAQDeviceHandler.h"
#include "easylogging++.h"
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>

class SerialPortHandler : public DAQDeviceHandler {
public:
    SerialPortHandler(std::string filepath);
    void startAcquisition() override;
    std::map<unsigned int, std::vector<double>> getLatestData() override;
    void stopAcquisition() override;
private:
    std::string filepath;
    std::ifstream serialport;
};

