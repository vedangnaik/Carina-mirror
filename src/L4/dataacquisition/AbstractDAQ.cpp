#include "AbstractDAQ.h"

AbstractDAQ::AbstractDAQ(const std::string deviceID, const unsigned int numChannels, const std::pair<std::array<double, 5>, std::array<double, 5>> calibrationPoints)
    : deviceID{deviceID}, numChannels{numChannels}
{
    this->calibrate(calibrationPoints);
}

void
AbstractDAQ::calibrate(std::pair<std::array<double, 5>, std::array<double, 5>> calibrationPoints)
{
    const auto& voltages = calibrationPoints.first;
    const auto& units = calibrationPoints.second;

    double meanVoltages = std::accumulate(voltages.begin(), voltages.end(), 0.0) / voltages.size();
    double meanUnits = std::accumulate(units.begin(), units.end(), 0.0) / units.size();
    double numerator = 0;
    double denominator = 0;
    for (size_t i = 0; i < voltages.size(); i++) {
        numerator += (voltages.at(i) - meanVoltages) * (units.at(i) - meanUnits);
        denominator += std::pow((voltages.at(i) - meanVoltages), 2);
    }
    this->slope = numerator / denominator;
    this->y_intercept = meanUnits - (slope * meanVoltages);
}
