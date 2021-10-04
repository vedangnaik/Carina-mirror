#include "ConcreteSensorFactory.h"

ConcreteSensorFactory::ConcreteSensorFactory() {
#ifdef ULDAQ_AVAILABLE
    this->discoverMCCDAQs();
#endif
}

Sensor*
ConcreteSensorFactory::createSensor(const std::string& id, const QVariantMap& args) {
    if (!args.contains("type")) {
        throw std::domain_error(id + ": Sensor must have a type.");
    }
    std::string type = args["type"].toString().toStdString();

    if (type == "DummySensor") {
        return this->createDummySensor(id, args);
    }
    else if (type == "AnalogMCCDAQSensor") {
        #ifdef ULDAQ_AVAILABLE
        return this->createAnalogMCCDAQSensor(id, args);
        #else
        throw std::domain_error(id + ": This Carina has not been compiled to support AnalogMCCDAQSensors. Please recompile with the -DULDAQ_AVAILABLE flag and ensure uldaq.h is available on your platform.");
        #endif
    }

    throw std::domain_error(id + ": Type '" + type + "' is invalid or not supported.");
}

Sensor*
ConcreteSensorFactory::createDummySensor(const std::string &id, const QVariantMap &args) {
    return new DummySensor(id, Helpers::parseCalibrationPointsFromArgs(id, args));
}

#ifdef ULDAQ_AVAILABLE
Sensor*
ConcreteSensorFactory::createAnalogMCCDAQSensor(const std::string &id, const QVariantMap &args) {
    // Retrieve the DAQDeviceDescriptorUniqueId. See https://www.mccdaq.com/PDFs/Manuals/UL-Linux/c/struct_daq_device_descriptor.html#a4e17bf9c02805011a7b5b02c4944f031.
    Helpers::checkForKeyAndConversionValidity(args, "uniqueID", QMetaType::QString, id + ": Analog MCCDAQ sensor must contain a valid numeric MCC device ID 'uniqueID'.");
    std::string uniqueID = args["uniqueID"].toString().toStdString();

    // Retrieve the channel this is connected to
    Helpers::checkForKeyAndConversionValidity(args, "channel", QMetaType::UInt, id + ": Analog MCCDAQ sensor must contain a valid positive integer 'channel'.");
    unsigned int channelConnectedTo = args["channel"].toUInt();

    // Iterate through all connected DAQs and check if any of them match the provided uniqueID. If so, create it.
    for (const auto& p : this->discoveredMCCDAQs) {
        if (p.first == uniqueID) {
            return new AnalogMCCDAQSensor(id, Helpers::parseCalibrationPointsFromArgs(id, args), p.second, channelConnectedTo);
        }
    }

    throw std::runtime_error(uniqueID + ": No MCC device with this handle found.");
}
#endif

#ifdef ULDAQ_AVAILABLE
void
ConcreteSensorFactory::discoverMCCDAQs() {
    // Get the number and descriptors of connected devices here.
    std::vector<DaqDeviceDescriptor> devDescriptors;
    unsigned int numDAQDevicesDetected = 0;
    UlError err = ulGetDaqDeviceInventory(ANY_IFC, devDescriptors.data(), &numDAQDevicesDetected);

    // No DAQS are found, complain.
    if (numDAQDevicesDetected == 0) {
        throw std::runtime_error("Unable to find any connected MCC devices.");
    }

    // Populate the vector of descriptors by calling this function again with the right number of connected DAQs.
    devDescriptors.reserve(numDAQDevicesDetected);
    err = ulGetDaqDeviceInventory(ANY_IFC, devDescriptors.data(), &numDAQDevicesDetected);
    if (err != ERR_NO_ERROR) {
        throw std::runtime_error("Failed to get inventory of connected MCC devices. Please check your physical connections and your platform's uldaq library.");
    }

    // Connect to all the discovered DAQs here.
    for (unsigned int i = 0; i < numDAQDevicesDetected; i++) {
        this->discoveredMCCDAQs.insert({
            std::string(devDescriptors[i].uniqueId),
            std::make_shared<MCCDAQHandler>(devDescriptors[i])
        });
    }
}
#endif