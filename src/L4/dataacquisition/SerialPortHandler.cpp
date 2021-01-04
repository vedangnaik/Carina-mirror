#include "SerialPortHandler.h"

SerialPortHandler::SerialPortHandler(std::string filepath) : filepath{filepath} {
    std::ifstream test(filepath);
    if (!test.is_open()) {
        LOG(ERROR) << "This arduino filepath could not be opened.";
        // throw exception here
    }
}

void SerialPortHandler::startAcquisition() {
    this->serialport = std::ifstream(this->filepath);
}

void SerialPortHandler::stopAcquisition() {
    this->serialport.close();
}

std::map<unsigned int, std::vector<double>> SerialPortHandler::getLatestData() {
    if (this->serialport.is_open()) {
        std::string line, T1, T2;
        std::getline(this->serialport, line);
        std::istringstream iss(line);
        iss >> T1 >> T2;
        LOG(INFO) << "T1: " << T1 << " " << "T2: " << T2;

        double t1, t2;
        try {
            t1 = std::stod(T1);
            t2 = std::stod(T2);
        } catch (std::exception& e) {
            t1 = nanf("0");
            t2 = nanf("0");
        }

        std::map<unsigned int, std::vector<double>> values = {
            {0, {t1}},
            {1, {t2}}
        };
        return values;
    } else {
        // shit, throw exception here
    }
}
