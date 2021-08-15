#include "DummyDAQ.h"

DummyDAQ::DummyDAQ(const std::string& id, const std::array<std::pair<double, double>, 5>& calibrationPoints)
    : Sensor(id, calibrationPoints)
{
    this->gen = std::mt19937(std::random_device{}());
    this->dis = std::uniform_real_distribution<>(-10.0, 10.0);
}

void DummyDAQ::startAcquisition() {}

void DummyDAQ::stopAcquisition() {}

double DummyDAQ::getLatestData() {
    this->latestValue = (this->slope * this->dis(this->gen)) + this->intercept;
    return this->latestValue;
}
