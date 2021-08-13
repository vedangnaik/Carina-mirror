#include "Sensor.h"

Sensor(const std::string id, const unsigned int numChannels)
    : id{id}, numChannels{numChannels}, latestValue{0.0}
{
    // Give identity calibration points and 1 slope, 0 intercept for all channels.
    std::array<std::pair<double, double>, 5> t {
        std::make_pair(1.0, 1.0), std::make_pair(2.0, 2.0), std::make_pair(3.0, 3.0), std::make_pair(4.0, 4.0), std::make_pair(5.0, 5.0)
    };
    this->calibrationPoints = std::vector<std::array<std::pair<double, double>, 5>>(this->numChannels, t);
    this->slopesAndIntercepts = std::vector<std::pair<double, double>>(this->numChannels, std::make_pair(1, 0));
}

void
Sensor::calibrate()
{
    for (unsigned int channel = 0; channel < this->numChannels; channel++) {
        const auto& channelCalibrationPoints = this->calibrationPoints.at(channel);
        double meanVoltages = 0;
        double meanUnits = 0;
        double numerator = 0;
        double denominator = 0;

        for (size_t i = 0; i < channelCalibrationPoints.size(); i++) {
            meanVoltages += channelCalibrationPoints.at(i).first;
            meanUnits += channelCalibrationPoints.at(i).second;
        }
        meanVoltages /= channelCalibrationPoints.size();
        meanUnits /= channelCalibrationPoints.size();

        for (size_t i = 0; i < channelCalibrationPoints.size(); i++) {
            numerator += (channelCalibrationPoints.at(i).first - meanVoltages) * (channelCalibrationPoints.at(i).second - meanUnits);
            denominator += std::pow((channelCalibrationPoints.at(i).first - meanVoltages), 2);
        }

        this->slopesAndIntercepts.at(channel).first = numerator / denominator;
        this->slopesAndIntercepts.at(channel).second = meanUnits - ((numerator / denominator) * meanVoltages);
    }
}
