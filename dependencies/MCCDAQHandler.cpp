#include "MCCDAQHandler.h"

MCCDAQHandler::MCCDAQHandler(const DaqDeviceDescriptor d)
    : d{d}
{
    // Allocate uniqueID
    this->uniqueID = std::string(d.uniqueId);

    // Create DAQ device
    this->handle = ulCreateDaqDevice(d);

    // Connect to DAQ
    UlError err = ulConnectDaqDevice(handle);
    if (err != ERR_NO_ERROR) {
        throw std::runtime_error(this->uniqueID + ": Unable to connect to MCCDAQ device.");
    }

    // Get number of channels
    err = ulAIGetInfo(this->handle, AI_INFO_NUM_CHANS_BY_MODE, AI_SINGLE_ENDED, &this->numChannels);
    if (err != ERR_NO_ERROR) {
        throw std::runtime_error(this->uniqueID + ": ulAIGetInfo number of channels error '" + std::to_string(err) + "'.");
    }

    // Get voltage range
    long long voltageRangeLL;
    err = ulAIGetInfo(handle, AI_INFO_SE_RANGE, 0, &voltageRangeLL);
    if (err != ERR_NO_ERROR) {
        throw std::runtime_error(this->uniqueID + ": ulAIGetInfo voltage range error '" + std::to_string(err) + "'.");
    }
    Range voltageRange = (Range)voltageRangeLL;

    // Allocate the temporary data buffer
    this->dataBuffer = std::make_unique<double[]>(this->numChannels * this->samplesPerChannel * sizeof(double));

    // Start acquisition
    err = ulAInScan(this->handle, 0, this->numChannels-1, AI_SINGLE_ENDED, voltageRange, this->samplesPerChannel, &this->rate, SO_CONTINUOUS, AINSCAN_FF_DEFAULT, this->dataBuffer.get());
    if (err != ERR_NO_ERROR) {
        throw std::runtime_error(this->uniqueID + ": ulAInScan error '" + std::to_string(err) + "'.");
    }
}

MCCDAQHandler::~MCCDAQHandler() {
    // Stop acquisition
    UlError err = ulAInScanStop(this->handle);
    if (err != ERR_NO_ERROR) { LOG(WARNING) << "ulAInScanStop Error: " << err; }

    // Free up daq device
    err = ulReleaseDaqDevice(this->handle);
    if (err != ERR_NO_ERROR) { LOG(WARNING) << "Failed to deallocate resources for MCC device with handle '" << std::to_string(this->handle) << "' ."; }
}