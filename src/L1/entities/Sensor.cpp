#include "Sensor.h"

Sensor::Sensor(const std::string& id, const std::array<std::pair<double, double>, 5>& calibrationPoints)
    : id{id}, calibrationPoints{calibrationPoints}
{
    this->calibrate();
}

void
Sensor::calibrate()
{
    double meanVoltages = 0;
    double meanUnits = 0;
    double numerator = 0;
    double denominator = 0;

    for (const auto& calibrationPoint : this->calibrationPoints) {
        meanVoltages += calibrationPoint.first;
        meanUnits += calibrationPoint.second;
    }
    meanVoltages /= this->calibrationPoints.size();
    meanUnits /= this->calibrationPoints.size();

    for (const auto& calibrationPoint : this->calibrationPoints) {
        numerator += (calibrationPoint.first - meanVoltages) * (calibrationPoint.second - meanUnits);
        denominator += std::pow((calibrationPoint.first - meanVoltages), 2);
    }

    this->slope = numerator / denominator;
    this->intercept = meanUnits - ((numerator / denominator) * meanVoltages);
}
