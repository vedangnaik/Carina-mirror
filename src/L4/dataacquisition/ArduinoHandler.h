#pragma once

#include "DAQDeviceHandler.h"
#include "easylogging++.h"
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>

class ArduinoHandler : public DAQDeviceHandler {
public:
    ArduinoHandler(std::string filepath);
    void startAcquisition() override;
    std::map<unsigned int, std::vector<double>> getLatestData() override;
    void stopAcquisition() override;
private:
    std::string filepath;
    std::ifstream arduinoInput;
};

