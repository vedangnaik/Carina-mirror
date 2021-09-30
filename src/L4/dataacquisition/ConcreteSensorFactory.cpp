#include "ConcreteSensorFactory.h"

ConcreteSensorFactory::ConcreteSensorFactory() {
#ifdef ULDAQ_AVAILABLE
    this->discoverAndConnectToMCCDAQs();
#endif
}

Sensor*
ConcreteSensorFactory::createSensor(const std::string& id, const QVariantMap& args) {
    if (!args.contains("type")) {
        throw std::domain_error(id + ": Sensor must have a type.");
    }

    std::string type = args["type"].toString().toStdString();
    if      (type == "DummySensor")         return this->createDummySensor(id, args);
    else if (type == "AnalogMCCDAQSensor")  return this->createAnalogMCCDAQSensor(id, args);
    else throw std::domain_error(id + ": Type '" + type + "' is invalid or not supported.");
}

Sensor*
ConcreteSensorFactory::createDummySensor(const std::string &id, const QVariantMap &args) {
    return new DummySensor(id, Helpers::parseCalibrationPointsFromArgs(id, args));
}

Sensor*
ConcreteSensorFactory::createAnalogMCCDAQSensor(const std::string &id, const QVariantMap &args) {
#ifdef ULDAQ_AVAILABLE
    // Retrieve the DAQDeviceDescriptorUniqueId. See https://www.mccdaq.com/PDFs/Manuals/UL-Linux/c/struct_daq_device_descriptor.html#a4e17bf9c02805011a7b5b02c4944f031.
    Helpers::checkForKeyAndConversionValidity(args, "handle", QMetaType::LongLong, id + ": Analog MCCDAQ sensor must contain a valid numeric MCC device handle 'handle'.");
    DaqDeviceHandle handle = (DaqDeviceHandle)args["handle"].toLongLong();

    // Retrieve the channel this is connected to
    Helpers::checkForKeyAndConversionValidity(args, "channel", QMetaType::UInt, id + ": Analog MCCDAQ sensor must contain a valid positive integer 'channel'.");
    unsigned int channelConnectedTo = args["channel"].toUInt();

    // Iterate through all connected DAQs and check if any of them match the provided handle. If so, create it.
    for (const auto& cachedHandle : this->cachedMCCDAQs) {
        if (cachedHandle == handle) {
            return new AnalogMCCDAQSensor(id, Helpers::parseCalibrationPointsFromArgs(id, args), handle, channelConnectedTo);
        }
    }

    throw std::runtime_error("No MCC device with handle '" + std::to_string(handle) + "' found.");
#else
    (void)args; // "Use" this to stop the compiler yelling.
    throw std::domain_error(id + ": This Carina has not been compiled to support AnalogMCCDAQSensors. Please recompile with the -DULDAQ_AVAILABLE flag and ensure uldaq.h is available on your platform.");
#endif
}

void
ConcreteSensorFactory::discoverAndConnectToMCCDAQs() {
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
        this->cachedMCCDAQs.push_back(ulCreateDaqDevice(devDescriptors[i]));
    }
}

void
ConcreteSensorFactory::disconnectFromMCCDAQs() {
    UlError err;
    for (const auto& handle : this->cachedMCCDAQs) {
        err = ulReleaseDaqDevice(handle);
        if (err != ERR_NO_ERROR) {
            LOG(WARNING) << "Failed to deallocate resources for MCC device with handle" << handle << ".";
        }
    }
}

ConcreteSensorFactory::~ConcreteSensorFactory() {
#ifdef ULDAQ_AVAILABLE
    this->disconnectFromMCCDAQs();
#endif
}
