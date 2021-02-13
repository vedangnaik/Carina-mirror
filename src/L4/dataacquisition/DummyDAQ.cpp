#include "DummyDAQ.h"

DummyDAQ::DummyDAQ(const std::string deviceID, const unsigned int numChannels, const std::vector<std::pair<std::array<double, 5>, std::array<double, 5>>> calibrationPoints)
    : AbstractDAQ(deviceID, numChannels, calibrationPoints)
{}

void DummyDAQ::startAcquisition() {}

void DummyDAQ::stopAcquisition() {}

std::vector<double> DummyDAQ::getLatestData() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-10.0, 10.0);
    std::vector<double> values(this->numChannels, std::nan("NaN"));
    for (unsigned int channel = 0; channel < this->numChannels; channel++) {
        double& slope = this->slopesAndIntercepts.at(channel).first;
        double& intercept = this->slopesAndIntercepts.at(channel).second;
        double val = (slope * dis(gen)) + intercept;
        values.at(channel) = val;
    }
    return values;
}
