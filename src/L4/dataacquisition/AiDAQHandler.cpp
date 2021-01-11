#ifdef ULDAQ_AVAILABLE

#include "AiDAQHandler.h"

AiDAQHandler::AiDAQHandler(std::string id, DaqDeviceHandle handle, unsigned int numChannels, Range voltageRange) : id{id}, handle{handle}, numChannels{numChannels}, voltageRange{voltageRange} {
    // connect DAQ
    UlError err = ulConnectDaqDevice(handle);
    if (err != ERR_NO_ERROR) { LOG(ERROR) << "ulConnectDaqDevice Error: " << err; }
    else { this->connected = true; }

    // allocate the temporary data buffer
    this->dataBuffer = std::make_unique<double[]>(this->numChannels * this->samplesPerChannel * sizeof(double));
}

AiDAQHandler::~AiDAQHandler() {
    this->stopAcquisition();
}

void AiDAQHandler::startAcquisition() {
    UlError err = ulAInScan(this->handle, 0, this->numChannels-1, this->aiim, this->voltageRange, this->samplesPerChannel, &this->rate, this->so, this->aisf, this->dataBuffer.get());
    if (err != ERR_NO_ERROR) { LOG(ERROR) << "ulAInScan Error: " << err; }
}

void AiDAQHandler::stopAcquisition() {
    UlError err = ulAInScanStop(this->handle);
    if (err != ERR_NO_ERROR) { LOG(ERROR) << "ulAInScanStop Error: " << err; }
}

std::vector<double> AiDAQHandler::getLatestData() {
    ScanStatus status;
    TransferStatus transferStatus;
    UlError err = ulAInScanStatus(this->handle, &status, &transferStatus);
    if (err != ERR_NO_ERROR) { LOG(ERROR) << "ulAInScanStatus Error: " << err; }

    int connected = 0;
    err = ulIsDaqDeviceConnected(this->handle, &connected);
    if (err != ERR_NO_ERROR) { LOG(ERROR) << "ulIsDaqDeviceConnected Error: " << err; }

    std::vector<double> values(std::nan("NaN"), this->numChannels);
    if (status == SS_RUNNING && connected != 0) {
        // Report average of all samples seen since last run.
        for (unsigned int i = 0; i < this->numChannels; i++) {
            values.assign(i, std::accumulate(&this->dataBuffer[i], &this->dataBuffer[i] + this->samplesPerChannel, 0.0) / this->samplesPerChannel);
        }
    } else {
        LOG(ERROR) << "DAQ device ID '" << this->id << "' cannot be accessed. Reporting all NaN.";
    }
    return values;
}

#endif
