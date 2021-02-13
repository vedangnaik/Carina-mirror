#include "AbstractDAQ.h"

AbstractDAQ::AbstractDAQ(const std::string deviceID, const unsigned int numChannels, const std::vector<std::pair<std::array<double, 5>, std::array<double, 5>>> calibrationPoints)
    : deviceID{deviceID}, numChannels{numChannels}
{
    this->slopesAndIntercepts = std::vector<std::pair<double, double>>(this->numChannels);
    this->calibrate(calibrationPoints);
}

void
AbstractDAQ::calibrate(std::vector<std::pair<std::array<double, 5>, std::array<double, 5>>> calibrationPoints)
{
    for (unsigned int channel = 0; channel < this->numChannels; channel++) {
        const auto& voltages = calibrationPoints.at(channel).first;
        const auto& units = calibrationPoints.at(channel).second;

        double meanVoltages = std::accumulate(voltages.begin(), voltages.end(), 0.0) / voltages.size();
        double meanUnits = std::accumulate(units.begin(), units.end(), 0.0) / units.size();
        double numerator = 0;
        double denominator = 0;
        for (size_t i = 0; i < voltages.size(); i++) {
            numerator += (voltages.at(i) - meanVoltages) * (units.at(i) - meanUnits);
            denominator += std::pow((voltages.at(i) - meanVoltages), 2);
        }

        this->slopesAndIntercepts.at(channel).first = numerator / denominator;
        this->slopesAndIntercepts.at(channel).second = meanUnits - ((numerator / denominator) * meanVoltages);
    }
}
