#include "DummyDAQ.h"

DummyDAQ::DummyDAQ(const std::string deviceID, const unsigned int numChannels)
    : AbstractDAQ(deviceID, numChannels)
{
    this->gen = std::mt19937(std::random_device{}());
    this->dis = std::uniform_real_distribution<>(-10.0, 10.0);

}

void DummyDAQ::startAcquisition() {}

void DummyDAQ::stopAcquisition() {}

std::vector<double> DummyDAQ::getLatestData() {
    std::vector<double> values(this->numChannels, std::nan("NaN"));
    for (unsigned int channel = 0; channel < this->numChannels; channel++) {
        double& slope = this->slopesAndIntercepts.at(channel).first;
        double& intercept = this->slopesAndIntercepts.at(channel).second;
        double val = (slope * this->dis(this->gen)) + intercept;
        values.at(channel) = val;
    }
    return values;
}
