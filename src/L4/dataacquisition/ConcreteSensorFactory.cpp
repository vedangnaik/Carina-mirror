#include "ConcreteSensorFactory.h"

Sensor* ConcreteSensorFactory::createSensor(const std::string& id, const QVariantMap& args) {
    if (args["type"] == "DummySensor") {
        if (!args.contains("calibration")) { /* TODO: Throw exception */ }
        return new DummySensor(id, parseCalibrationPointsFromArgs(args));
    }
#ifdef ULDAQ_AVAILABLE
    else if (args["type"] == "AnalogMCCDAQSensor") {
        // connect DAQ
        UlError err = ulConnectDaqDevice(handle);
        if (err != ERR_NO_ERROR) { LOG(ERROR) << "ulConnectDaqDevice Error: " << err; }
        return new AnalogMCCDAQSensor(id, parseCalibrationPointsFromArgs(args), )
    }
#endif
    else {
        // TODO: Exception
        return nullptr;
    }
}

std::array<std::pair<double, double>, 5>
ConcreteSensorFactory::parseCalibrationPointsFromArgs(const QVariantMap &args) {
    std::array<std::pair<double, double>, 5> calibrationPoints;
    for (int i = 0; i < 5; i++) {
        std::pair<double, double> t{
            args["calibration"].toJsonArray().at(i).toArray().at(0).toDouble(),
            args["calibration"].toJsonArray().at(i).toArray().at(1).toDouble()
        };
        calibrationPoints.at(i) = t;
    }
    return calibrationPoints;
}
