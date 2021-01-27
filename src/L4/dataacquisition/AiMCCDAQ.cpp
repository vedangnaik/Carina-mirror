#ifdef ULDAQ_AVAILABLE

#include "AiMCCDAQ.h"

AiMCCDAQ::AiMCCDAQ(const std::string deviceID, const unsigned int numChannels, const std::pair<std::vector<double>, std::vector<double> > calibrationPoints, DaqDeviceHandle handle, Range voltageRange)
    : AbstractDAQ(deviceID, numChannels, calibrationPoints), handle{handle}, voltageRange{voltageRange}
{
    // connect DAQ
    UlError err = ulConnectDaqDevice(handle);
    if (err != ERR_NO_ERROR) { LOG(ERROR) << "ulConnectDaqDevice Error: " << err; }

    // allocate the temporary data buffer
    this->dataBuffer = std::make_unique<double[]>(this->numChannels * this->samplesPerChannel * sizeof(double));
}

AiMCCDAQ::~AiMCCDAQ() {
    this->stopAcquisition();
}

void AiMCCDAQ::startAcquisition() {
    UlError err = ulAInScan(this->handle, 0, this->numChannels-1, this->aiim, this->voltageRange, this->samplesPerChannel, &this->rate, this->so, this->aisf, this->dataBuffer.get());
    if (err != ERR_NO_ERROR) { LOG(ERROR) << "ulAInScan Error: " << err; }
}

void AiMCCDAQ::stopAcquisition() {
    UlError err = ulAInScanStop(this->handle);
    if (err != ERR_NO_ERROR) { LOG(ERROR) << "ulAInScanStop Error: " << err; }
}

std::vector<double> AiMCCDAQ::getLatestData() {
    ScanStatus status;
    TransferStatus transferStatus;
    UlError err = ulAInScanStatus(this->handle, &status, &transferStatus);
    if (err != ERR_NO_ERROR) { LOG(ERROR) << "ulAInScanStatus Error: " << err; }

    int connected = 0;
    err = ulIsDaqDeviceConnected(this->handle, &connected);
    if (err != ERR_NO_ERROR) { LOG(ERROR) << "ulIsDaqDeviceConnected Error: " << err; }

    std::vector<double> values(this->numChannels, std::nan("NaN"));
    if (status == SS_RUNNING && connected != 0) {
        // Report average of all samples seen since last run.
        for (unsigned int i = 0; i < this->numChannels; i++) {
            // I was debating whether to use operator[] or .at(). But since we are guaranteed that i will be in range,
            // I don't think it's needed. But maybe it's better to have?
            values[i] = std::accumulate(&this->dataBuffer[i], &this->dataBuffer[i] + this->samplesPerChannel, 0.0) / this->samplesPerChannel;
        }
    } else {
        LOG(ERROR) << "DAQ device ID '" << this->deviceID << "' cannot be accessed. Reporting all NaN.";
    }
    return values;
}

#endif
