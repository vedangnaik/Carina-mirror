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
    SerialPortDAQ(std::string id, std::string serialportPath, unsigned int numChannels);
    void startAcquisition() override;
    std::vector<double> getLatestData() override;
    void stopAcquisition() override;
private:
    const std::string serialportPath;
    std::ifstream serialport;
};

