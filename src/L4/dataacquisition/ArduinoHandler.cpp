#include "ArduinoHandler.h"

ArduinoHandler::ArduinoHandler(std::string filepath) : filepath{filepath} {
    std::ifstream test(filepath);
    if (!test.is_open()) {
        LOG(ERROR) << "This arduino filepath could not be opened.";
        // throw exception here
    }
}

void ArduinoHandler::startAcquisition() {
    this->arduinoInput = std::ifstream(this->filepath);
}

void ArduinoHandler::stopAcquisition() {
    this->arduinoInput.close();
}

std::map<unsigned int, std::vector<double>> ArduinoHandler::getLatestData() {
    if (this->arduinoInput.is_open()) {
        std::string line, T1, T2;
        std::getline(this->arduinoInput, line);
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
