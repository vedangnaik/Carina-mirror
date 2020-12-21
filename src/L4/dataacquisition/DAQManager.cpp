#include "DAQManager.h"

DAQManager::DAQManager(SVGIC& svgic) : svgic{svgic} {
#ifdef __linux__
    std::vector<DaqDeviceDescriptor> devDescriptors;

    // Get the number of connected devices here.
    unsigned int numDAQDevicesDetected = 0;
    UlError err = ulGetDaqDeviceInventory(this->DAQDeviceInterfaceType, devDescriptors.data(), &numDAQDevicesDetected);
    // This will trip, but can be ignored safely (I think)
    if (err != ERR_NO_ERROR) { /*shit */ std::cout << "ulGetDaqDeviceInventory Error: " << err << std::endl; }


    // Create DAQ instances
    if (numDAQDevicesDetected == 0) {
        // shit
        std::cout << "No DAQ devices detected\n";
    }
    else {
        // Get the device descriptors here
        devDescriptors.reserve(numDAQDevicesDetected);
        UlError err = ulGetDaqDeviceInventory(this->DAQDeviceInterfaceType, devDescriptors.data(), &numDAQDevicesDetected);
        if (err != ERR_NO_ERROR) { /*shit */ std::cout << "ulGetDaqDeviceInventory Error: " << err << std::endl; }

        // Create DAQ objects here
        for (unsigned int i = 0; i < numDAQDevicesDetected; i++) {
            // Create the DAQ object
            DaqDeviceHandle handle = ulCreateDaqDevice(devDescriptors[i]);

            // Check if it's analog input; other types can be checked for here
            long long aiSupported;
            UlError err = ulDevGetInfo(handle, DEV_INFO_HAS_AI_DEV, 0, &aiSupported);
            if (err != ERR_NO_ERROR) { /*shit */ std::cout << "ulDevGetInfo Error: " << err << std::endl; }

            // Get num channels
            long long numChannels;
            err = ulAIGetInfo(handle, AI_INFO_NUM_CHANS, 0, &numChannels);
            if (err != ERR_NO_ERROR) { /*shit */ std::cout << "ulAIGetInfo Error: " << err << std::endl; }
            if (aiSupported < 1) { /* shit */ std::cout << "Analog input not supported: " << aiSupported << std::endl; }

            // get voltage range
            long long voltageRange;
            err = ulAIGetInfo(handle, AI_INFO_SE_RANGE, 0, &voltageRange);
            if (err != ERR_NO_ERROR) { /* shit */ std::cout << "ulAIGetInfo Error: " << err << std::endl; }

            // Create DAQHandler for this DAQ
            if (aiSupported != 0) {
                std::cout << "Handle: " << handle << "\nChannels: " << numChannels << "\nRange: " << voltageRange << "\n";
                this->DAQDevices.push_back(new AiDAQHandler(handle, numChannels, (Range)voltageRange));
            // create other DAQs here
            } else {
                std::cout << "This aiSupported value: " << aiSupported << " is not supported right now.\n";
            }
        }
    }
#else
    // push back dummy daq here
#endif

    // Create timer to start reading from DAQs here
    this->DAQReadTimer = new QTimer(this);
    this->DAQReadTimer->start(1000);
}

void DAQManager::startAcquisition() {
    for (const auto& d : this->DAQDevices) {
        d->startAcquisition();
    }
    connect(this->DAQReadTimer, &QTimer::timeout, this, &DAQManager::getLatestData);
}

void DAQManager::stopAcquisition() {
    disconnect(this->DAQReadTimer, &QTimer::timeout, this, &DAQManager::getLatestData);
    for (const auto& d : this->DAQDevices) {
        d->stopAcquisition();
    }
}

void DAQManager::getLatestData() {
    // figure out here how to correclty hook up the channels from all DAQs into ids for defined sensors
    // output random values for now
    for (std::string id : this->svgic.getSensorIDs()) {
        this->svgic.updateValue(id, rand() % 1000);
    }
}
