#include "AbstractDAQ.h"

AbstractDAQ::AbstractDAQ(const std::string deviceID, const unsigned int numChannels, const std::pair<std::vector<double>, std::vector<double>> calibrationPoints)
    : deviceID{deviceID}, numChannels{numChannels}
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
    slope = numerator / denominator;
    y_intercept = meanUnits - (slope * meanVoltages);
};
