#pragma once

#include "DAQDeviceHandler.h"
#include "easylogging++.h"
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>

class SerialPortHandler : public DAQDeviceHandler {
public:
    SerialPortHandler(std::string id, std::string filepath, unsigned int numChannels);
    void startAcquisition() override;
    std::map<unsigned int, std::vector<double>> getLatestData() override;
    void stopAcquisition() override;
private:
    std::string id;
    std::string serialportPath;
    unsigned int numChannels;
    std::ifstream serialport;
};

