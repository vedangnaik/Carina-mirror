#pragma once

#include "AbstractDAQ.h"
#include "easylogging++.h"
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>

class SerialPortDAQ : public Sensor
{
public:
    SerialPortDAQ(const std::string deviceID, const unsigned int numChannels, std::string serialportPath);
    void startAcquisition() override;
    std::vector<double> getLatestData() override;
    void stopAcquisition() override;
private:
    const std::string serialportPath;
    std::ifstream serialport;
};

