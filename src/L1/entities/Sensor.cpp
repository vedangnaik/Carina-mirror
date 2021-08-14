#include "Sensor.h"

Sensor::Sensor(const std::string id)
    : id{id}
{
    // Give identity calibration points and 1 slope, 0 intercept for all channels.
    this->calibrationPoints = {
        std::make_pair(1.0, 1.0), std::make_pair(2.0, 2.0), std::make_pair(3.0, 3.0), std::make_pair(4.0, 4.0), std::make_pair(5.0, 5.0)
    };
}

void
Sensor::calibrate()
{
    double meanVoltages = 0;
    double meanUnits = 0;
    double numerator = 0;
    double denominator = 0;

    for (size_t i = 0; i < this->calibrationPoints.size(); i++) {
        meanVoltages += this->calibrationPoints.at(i).first;
        meanUnits += this->calibrationPoints.at(i).second;
    }
    meanVoltages /= this->calibrationPoints.size();
    meanUnits /= this->calibrationPoints.size();

    for (size_t i = 0; i < this->calibrationPoints.size(); i++) {
        numerator += (this->calibrationPoints.at(i).first - meanVoltages) * (this->calibrationPoints.at(i).second - meanUnits);
        denominator += std::pow((this->calibrationPoints.at(i).first - meanVoltages), 2);
    }

    this->slope = numerator / denominator;
    this->intercept = meanUnits - ((numerator / denominator) * meanVoltages);
}
