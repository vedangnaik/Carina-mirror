#include "DAQManager.h"

DAQManager::DAQManager(ClocksModule& cm, SVGIC& svgic) : cm{cm}, svgic{svgic} {
    DaqDeviceDescriptor devDiscs[this->maxDAQDevices];
    unsigned int numDAQDevicesDetected = 0;
    UlError err = ulGetDaqDeviceInventory(this->DAQDeviceInterfaceType, devDiscs, &numDAQDevicesDetected);
    if (err != ERR_NO_ERROR) { /* shit */ }

    if (numDAQDevicesDetected == 0) {
        // shit
    }
    else {
        for (unsigned int i = 0; i < numDAQDevicesDetected; i++) {
            // Create the DAQ object
            DaqDeviceHandle handle = ulCreateDaqDevice(devDiscs[i]);

            // Check if it's analog input; other types can be checked for here
            long long aiSupported;
            UlError err = ulDevGetInfo(handle, DEV_INFO_HAS_AI_DEV, 0, &aiSupported);
            if (err != ERR_NO_ERROR) { /* shit */ }

            // Get num channels
            long long numChannels;
            err = ulAIGetInfo(handle, AI_INFO_NUM_CHANS, 0, &numChannels);
            if (err != ERR_NO_ERROR) { /* shit */ }
            if (aiSupported < 1) { /* shit */ }

            // get voltage range
            long long numSERanges;
            long long voltageRange;
            err = ulAIGetInfo(handle, AI_INFO_NUM_SE_RANGES, 0, &numSERanges);
            if (err != ERR_NO_ERROR) { /* shit */ }
            if (numSERanges < 0) { /* shit */ }
            err = ulAIGetInfo(handle, AI_INFO_SE_RANGE, numSERanges, &voltageRange);
            if (err != ERR_NO_ERROR) { /* shit */ }

            // Create DAQHandler for this DAQ
            if (aiSupported != 0) {
                this->DAQDevices.push_back(new AiDAQHandler(handle, numChannels, (Range)voltageRange));
            // create other DAQs here
            } else {
                // message 'not supported' or something
            }
        }
    }
}

void DAQManager::startAcquisition() {
    for (const auto& d : this->DAQDevices) {
        d->startAcquisition();
    }
    connect(this->cm.HundredMsTimer, &QTimer::timeout, this, &DAQManager::getLatestData);
}

void DAQManager::stopAcquisition() {
    disconnect(this->cm.HundredMsTimer, &QTimer::timeout, this, &DAQManager::getLatestData);
    for (const auto& d : this->DAQDevices) {
        d->stopAcquisition();
    }
}

void DAQManager::getLatestData() {
    // figure out here how to correclty hook up the channels from all DAQs into ids for defined sensors
    // output random values for now
    for (std::string id : this->svgic.getSensorIDs()) {
        this->svgic.updateValue(id, random() % 1000);
    }
}
