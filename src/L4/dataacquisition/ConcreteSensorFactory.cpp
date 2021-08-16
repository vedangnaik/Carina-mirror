#include "ConcreteSensorFactory.h"

Sensor* ConcreteSensorFactory::createSensor(const std::string& id, const QVariantMap& args) {
    // TODO: Check for required keys and throw exceptions as needed.
    if (args["type"] == "DummySensor") {
        return new DummySensor(id, parseCalibrationPointsFromArgs(args));
    }
    else if (args["type"] == "AnalogMCCDAQSensor") {
#ifdef ULDAQ_AVAILABLE
        // connect DAQ
        DaqDeviceHandle handle = (DaqDeviceHandle)args["handle"].toLongLong();
        UlError err = ulConnectDaqDevice(handle);
        if (err != ERR_NO_ERROR) { LOG(ERROR) << "ulConnectDaqDevice Error: " << err; }
        return new AnalogMCCDAQSensor(id, parseCalibrationPointsFromArgs(args), args["channel"].toUInt(), handle);
#else
        LOG(WARNING) << "MCCDAQs are not supported for this platform."
        // TODO: Exception
#endif
    }
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
