#include "AiDAQHandler.h"

AiDAQHandler::AiDAQHandler(DaqDeviceHandle handle, unsigned int numChannels, Range voltageRange) : handle{handle}, numChannels{numChannels}, voltageRange{voltageRange} {
    // connect DAQ
    UlError err = ulConnectDaqDevice(handle);
    if (err != ERR_NO_ERROR) { /* shit */ }
    else { this->connected = true; }

    // allocate the temporary data buffer
    this->dataBuffer = (double*)malloc(this->numChannels * this->samplesPerChannel * sizeof(double));
}

AiDAQHandler::~AiDAQHandler() {
    free(this->dataBuffer);
}

void AiDAQHandler::startAcquisition() {
    UlError err = ulAInScan(this->handle, 0, this->numChannels-1, this->aiim, this->voltageRange, this->samplesPerChannel, &this->rate, this->so, this->aisf, this->dataBuffer);
    if (err != ERR_NO_ERROR) { /* shit */ }
}

void AiDAQHandler::stopAcquisition() {
    UlError err = ulAInScanStop(this->handle);
    if (err != ERR_NO_ERROR) { /* shit */ }
}

std::map<unsigned int, std::vector<double>> AiDAQHandler::getLatestData() {
    ScanStatus status;
    TransferStatus transferStatus;
    UlError err = ulAInScanStatus(this->handle, &status, &transferStatus);
    if (err != ERR_NO_ERROR) { /* shit */ }

    int connected = 0;
    err = ulIsDaqDeviceConnected(this->handle, &connected);
    if (err != ERR_NO_ERROR) { /* shit */ }

    if (status == SS_RUNNING && connected != 0) {
        std::map<unsigned int, std::vector<double>> values;
        for (unsigned int i = 0; i < this->numChannels; i++) {
            values.insert(std::make_pair(i, std::vector<double>{}));
            for (unsigned int j = 0; j < this->samplesPerChannel; j++) {
                values.at(i).push_back(this->dataBuffer[(i * this->samplesPerChannel) + j]);
            }
        }
        return values;
    } else {
        // shit
    }
}