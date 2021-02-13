#include "SerialPortDAQ.h"

SerialPortDAQ::SerialPortDAQ(const std::string deviceID, const unsigned int numChannels, const std::pair<std::array<double, 5>, std::array<double, 5>> calibrationPoints, std::string serialportPath)
    : AbstractDAQ(deviceID, numChannels, calibrationPoints), serialportPath{serialportPath}
{
    std::ifstream test(serialportPath);
    if (!test.is_open()) {
        LOG(ERROR) << "SerialPortHandler error - device id: " << this->deviceID << ", num channels: " << this->numChannels << ", serial port path: " << this->serialportPath << "; Serial port path could not be opened.";
        // throw exception here
    }
}

void SerialPortDAQ::startAcquisition() {
    this->serialport = std::ifstream(this->serialportPath);
}

void SerialPortDAQ::stopAcquisition() {
    this->serialport.close();
}

std::vector<double> SerialPortDAQ::getLatestData() {
    std::vector<double> values(this->numChannels, std::nan("NaN"));
    if (this->serialport.is_open()) {
        std::string line;
        std::getline(this->serialport, line);
        std::istringstream iss(line);
        for (unsigned int i = 0; i < this->numChannels; i++) {
            std::string sval;
            iss >> sval;
            try {
                values[i] = std::stod(sval);
            } catch (std::exception& e) {
                LOG(ERROR) << "DAQ device ID '" << this->deviceID << "': channel value '" << i << "' could not be read. Reporting NaN.";
            }
        }
    } else {
        LOG(ERROR) << "DAQ device ID '" << this->deviceID << "' cannot be accessed. Reporting all NaN.";
    }
    return values;
}
