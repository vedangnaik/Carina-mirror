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



Sensor*
ConcreteSensorFactory::createDummySensor(const std::string &id, const QVariantMap &args) {
    return new DummySensor(id, Helpers::parseCalibrationPointsFromArgs(id, args));
}

Sensor*
ConcreteSensorFactory::createAnalogMCCDAQSensor(const std::string &id, const QVariantMap &args) {
#ifdef ULDAQ_AVAILABLE
    // Retrieve the DAQDeviceDescriptorUniqueId. See https://www.mccdaq.com/PDFs/Manuals/UL-Linux/c/struct_daq_device_descriptor.html#a4e17bf9c02805011a7b5b02c4944f031.
    Helpers::checkForKeyAndConversionValidity(args, "uniqueId", QMetaType::QString, id + ": Analog MCCDAQ sensor must contain a valid numeric MCC device ID 'uniqueId'.");
    std::string uniqueId = args["uniqueId"].toString().toStdString();

    // Retrieve the channel this is connected to
    Helpers::checkForKeyAndConversionValidity(args, "channel", QMetaType::UInt, id + ": Analog MCCDAQ sensor must contain a valid positive integer 'channel'.");
    unsigned int channelConnectedTo = args["channel"].toUInt();

    // Get the number and descriptors of connected devices here.
    std::vector<DaqDeviceDescriptor> devDescriptors;
    unsigned int numDAQDevicesDetected = 0;
    UlError err = ulGetDaqDeviceInventory(ANY_IFC, devDescriptors.data(), &numDAQDevicesDetected);

    // No DAQS are found, complain.
    if (numDAQDevicesDetected == 0) {
        throw std::runtime_error("Unable to find any connected MCC devices.");
    }

    // Populate the vector of descriptors by calling this function again with the right number of connected daqs.
    devDescriptors.reserve(numDAQDevicesDetected);
    err = ulGetDaqDeviceInventory(ANY_IFC, devDescriptors.data(), &numDAQDevicesDetected);
    if (err != ERR_NO_ERROR) {
        throw std::runtime_error("Failed to get inventory of connected MCC devices. Please check your physical connections and platform's uldaq library.");
    }

    // Iterate through all connected devices and check if any of them match the provided uniqueId. If so, create it.
    for (unsigned int i = 0; i < numDAQDevicesDetected; i++) {
        if (std::string(devDescriptors[i].uniqueId) == uniqueId) {
            return new AnalogMCCDAQSensor(id, Helpers::parseCalibrationPointsFromArgs(id, args), devDescriptors[i], channelConnectedTo);
        }
    }

    throw std::runtime_error("No MCC device with uniqueId '" + uniqueId + "' found.");
#else
    throw std::domain_error(id + ": This Carina has not been compiled to support AnalogMCCDAQSensors. Please recompile with the -DULDAQ_AVAILABLE flag and ensure uldaq.h is available on your platform.");
#endif
}

// Empty for now. In case this factory has some state that has lifetime shorter than the program's lifetime, use this function to clear it.
void ConcreteSensorFactory::resetFactory() {}
