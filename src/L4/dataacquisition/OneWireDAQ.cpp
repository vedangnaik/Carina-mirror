/*
#include "OneWireDAQ.h"
#include <fstream>
#include <sstream>
#include <filesystem>

using directory_iterator = std::filesystem::directory_iterator;

OneWireDAQ::OneWireDAQ(const std::string deviceID, int gpioPin, const unsigned int numChannels)
    : AbstractDAQ(deviceID, numChannels)
{
    this->gpioPin = gpioPin;
    this->w1_file = "/sys/bus/w1/devices/w1_bus_master" + std::to_string(gpioPin) + "/";

    int n =w w1_file.size();
    bool foundDevice = false;
    for (const auto& dir : directory_iterator(this->w1_file)){
        std::string filepath = dir.path().string();
        if (filepath[n] == '3' && filepath[n+1] == 'b'){
            this->w1_file = filepath;
            foundDevice = true;
        }
    }
    if (foundDevice){
         this->w1_file += "/w1_slave";
    }
    else{
        LOG(ERROR) << "Could not find 1-wire folder for DAQ device ID '" << this->deviceID << "'.";
    }

}
void OneWireDAQ::startAcquisition() {
    // check if file can be opened/exists
    bool foundDevice = std::filesystem::exists(this->w1_file);
    if (!foundDevice){
        LOG(ERROR) << this->w1_file << " for DAQ device ID '" << this->deviceID << "' cannot be found.";
    }
    return;
}

void OneWireDAQ::stopAcquisition() {
    return;
}

std::vector<double> OneWireDAQ::getLatestData()
{
    std::vector<double> values(this->numChannels, std::nan("NaN"));

    if (std::filesystem::exists(this->w1_file)) {
        for (unsigned int channel = 0; channel < this->numChannels; channel++) {
            // code to parse the data from /w1_file into val
            int raw_data;
            double val;

            std::ifstream infile(this->w1_file);
            std::string line;
            // check if data is valid
            if(std::getline(infile, line)){
                if (line.substr(line.size()-3, 3) != "YES"){
                    LOG(ERROR) << "DAQ device ID '" << this->deviceID << "': channel value '" << channel << "' data not ready. Reporting NaN.";
                }
            }
            else{
                LOG(ERROR) << "DAQ device ID '" << this->deviceID << "': channel value '" << channel << "' could not be read. Reporting NaN.";
            }
            // read data
            if(std::getline(infile, line)){
                auto index = line.find("t=");
                if (index != std::string::npos){
                    int start_index = index + 2;
                    std::string temp_str = line.substr(start_index, line.size()-start_index);
                    std::istringstream iss(temp_str);
                    temp_str >> raw_data;
                    val = raw_data / 1000.0;
                }
                else{
                    LOG(ERROR) << "DAQ device ID '" << this->deviceID << "': channel value '" << channel << "' could not be read. Reporting NaN.";
                }
            }
            else{
                LOG(ERROR) << "DAQ device ID '" << this->deviceID << "': channel value '" << channel << "' could not be read. Reporting NaN.";
            }

            try {
                double& slope = this->slopesAndIntercepts.at(channel).first;
                double& intercept = this->slopesAndIntercepts.at(channel).second;
                values.at(channel) = (slope * val) + intercept;
            } catch (std::exception& e) { // fix log statement
                LOG(ERROR) << "DAQ device ID '" << this->deviceID << "': channel value '" << channel << "' could not be read. Reporting NaN.";
            }
        }
    } else {
        LOG(ERROR) << "DAQ device ID '" << this->deviceID << "' cannot be accessed. Reporting all NaN.";
    }

    return values;
}
*/
