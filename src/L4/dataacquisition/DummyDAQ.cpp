#include "DummyDAQ.h"

DummyDAQ::DummyDAQ(std::string deviceID, unsigned int numChannels) : AbstractDAQ(deviceID, numChannels) {}
void DummyDAQ::startAcquisition() {}
void DummyDAQ::stopAcquisition() {}

std::vector<double> DummyDAQ::getLatestData() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-500.0, 1000.0);
    std::vector<double> values;
    for (unsigned int _ = 0; _ < this->numChannels; _++) {
        values.push_back(dis(gen));
    }
    return values;
}
