// #include "SerialPortDAQ.h"

// SerialPortDAQ::SerialPortDAQ(const std::string deviceID, const unsigned int numChannels, std::string serialportPath)
//     : Sensor(deviceID, numChannels), serialportPath{serialportPath}
// {
//     std::ifstream test(serialportPath);
//     if (!test.is_open()) {
//         LOG(ERROR) << "SerialPortHandler error - device id: " << this->deviceID << ", num channels: " << this->numChannels << ", serial port path: " << this->serialportPath << "; Serial port path could not be opened.";
//         // throw exception here
//     }
// }

// void SerialPortDAQ::startAcquisition() {
//     this->serialport = std::ifstream(this->serialportPath);
// }

// void SerialPortDAQ::stopAcquisition() {
//     this->serialport.close();
// }

// std::vector<double> SerialPortDAQ::getLatestData() {
//     std::vector<double> values(this->numChannels, std::nan("NaN"));
//     if (this->serialport.is_open()) {
//         std::string line;
//         std::getline(this->serialport, line);
//         std::istringstream iss(line);
//         for (unsigned int channel = 0; channel < this->numChannels; channel++) {
//             std::string sval;
//             iss >> sval;
//             try {
//                 double& slope = this->slopesAndIntercepts.at(channel).first;
//                 double& intercept = this->slopesAndIntercepts.at(channel).second;
//                 values.at(channel) = (slope * std::stod(sval)) + intercept;
//             } catch (std::exception& e) {
//                 LOG(ERROR) << "DAQ device ID '" << this->deviceID << "': channel value '" << channel << "' could not be read. Reporting NaN.";
//             }
//         }
//     } else {
//         LOG(ERROR) << "DAQ device ID '" << this->deviceID << "' cannot be accessed. Reporting all NaN.";
//     }
//     return values;
// }
