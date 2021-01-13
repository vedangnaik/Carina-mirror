#include "SerialPortHandler.h"

SerialPortHandler::SerialPortHandler(std::string deviceID, std::string serialportPath, unsigned int numChannels) : AbstractDAQDeviceHandler(deviceID, numChannels), serialportPath{serialportPath} {
    std::ifstream test(serialportPath);
    if (!test.is_open()) {
        LOG(ERROR) << "This arduino filepath could not be opened.";
        // throw exception here
    }
}

void SerialPortHandler::startAcquisition() {
    this->serialport = std::ifstream(this->serialportPath);
}

void SerialPortHandler::stopAcquisition() {
    this->serialport.close();
}

std::vector<double> SerialPortHandler::getLatestData() {
    std::vector<double> values(std::nan("NaN"), this->numChannels);
    if (this->serialport.is_open()) {
        std::string line;
        std::getline(this->serialport, line);
        std::istringstream iss(line);
        for (unsigned int i = 0; i < this->numChannels; i++) {
            std::string sval;
            iss >> sval;
            try {
                values.assign(i, std::stod(sval));
            } catch (std::exception& e) {
                LOG(ERROR) << "DAQ device ID '" << this->deviceID << "': channel value '" << i << "' could not be read. Reporting NaN.";
            }
        }
    } else {
        LOG(ERROR) << "DAQ device ID '" << this->deviceID << "' cannot be accessed. Reporting all NaN.";
    }
    return values;
}
