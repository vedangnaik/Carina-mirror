#include "DummyDAQ.h"

DummyDAQ::DummyDAQ(const std::string id)
    : Sensor(id)
{
    this->gen = std::mt19937(std::random_device{}());
    this->dis = std::uniform_real_distribution<>(-10.0, 10.0);
}

void DummyDAQ::startAcquisition() {}

void DummyDAQ::stopAcquisition() {}

std::vector<double> DummyDAQ::getLatestData() {
    return (this->slope * this->dis(this->gen)) + this->intercept;
}
