#pragma once

#include "AbstractDAQ.h"
#include "easylogging++.h"
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>

class SerialPortDAQ : public AbstractDAQ
{
public:
    SerialPortDAQ(const std::string deviceID, const unsigned int numChannels, const std::pair<std::array<double, 5>, std::array<double, 5>> calibrationPoints);
    void startAcquisition() override;
    std::vector<double> getLatestData() override;
    void stopAcquisition() override;
private:
    const std::string serialportPath;
    std::ifstream serialport;
};

