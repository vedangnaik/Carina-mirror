#include "ConcreteSensorFactory.h"

std::unordered_map<std::string, Sensor* (*)(const std::string&, const QVariantMap&)> ConcreteSensorFactory::factoryMap = {
    {"DummySensor", &ConcreteSensorFactory::createDummySensor},
    {"AnalogMCCDAQSensor", &ConcreteSensorFactory::createAnalogMCCDAQSensor},
};

Sensor*
ConcreteSensorFactory::createSensor(const std::string& id, const QVariantMap& args) {
    if (!args.contains("type")) {
        throw std::domain_error(id + ": Sensor must have a type.");
    }

    std::string type = args["type"].toString().toStdString();
    if (factoryMap.count(type) == 0) {
        throw std::domain_error(id + ": Type '" + type + "' is invalid or not supported.");
    }

    return factoryMap[type](id, args);
}

std::array<std::pair<double, double>, 5>
ConcreteSensorFactory::parseCalibrationPointsFromArgs(const std::string& id, const QVariantMap &args) {
    Helpers::checkForKeyAndConversionValidity(args, "calibration", QMetaType::QJsonArray, id + ": Sensor must contain set of 5 valid calibration points.");

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

Sensor*
ConcreteSensorFactory::createDummySensor(const std::string &id, const QVariantMap &args) {
    return new DummySensor(id, parseCalibrationPointsFromArgs(id, args));
}

Sensor*
ConcreteSensorFactory::createAnalogMCCDAQSensor(const std::string &id, const QVariantMap &args) {
#ifdef ULDAQ_AVAILABLE
    // Retrieve the handle
    Helpers::checkForKeyAndConversionValidity(args, "handle", QMetaType::LongLong, id + ": Sensor must contain a valid numeric MCC device handle.");
    DaqDeviceHandle handle = (DaqDeviceHandle)args["handle"].toLongLong();

    // Retrieve the channel this is connected to
    Helpers::checkForKeyAndConversionValidity(args, "channel", QMetaType::UInt, id + ": Sensor must contain a valid positive integer channel to connect to.");
    unsigned int channelConnectedTo = args["channel"].toUInt();

    // If this DAQ has already been connected, leave it. Connecting twice appears to actually disconnect the DAQ.
    int isConnected;
    ulIsDaqDeviceConnected(handle, &isConnected);
    if (isConnected == 0) {
        UlError err = ulConnectDaqDevice(handle);
        if (err != ERR_NO_ERROR) {
            throw std::runtime_error(id + ": Unable to connect to MCC device with handle '" + std::to_string(handle) + "'.");
        }
    }

    // Create the thing
    return new AnalogMCCDAQSensor(id, parseCalibrationPointsFromArgs(id, args), channelConnectedTo, handle);
#else
    throw std::domain_error(id + ": This Carina has not been compiled to support AnalogMCCDAQSensors. Please consult the developers for further information.")
#endif
}
