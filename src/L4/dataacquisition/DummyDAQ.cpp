#include "DummyDAQ.h"

DummyDAQ::DummyDAQ(const std::string deviceID, const unsigned int numChannels, const std::pair<std::vector<double>, std::vector<double>> calibrationPoints)
    : AbstractDAQ(deviceID, numChannels, calibrationPoints)
{}

void DummyDAQ::startAcquisition() {}

void DummyDAQ::stopAcquisition() {}

std::vector<double> DummyDAQ::getLatestData() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-10.0, 10.0);
    std::vector<double> values;
    for (unsigned int _ = 0; _ < this->numChannels; _++) {
        double val = (this->slope * dis(gen)) + this->y_intercept;
        values.push_back(val);
    }
    return values;
}
