#ifdef ULDAQ_AVAILABLE

#include "AnalogMCCDAQSensor.h"

AnalogMCCDAQSensor::AnalogMCCDAQSensor(const std::string& deviceID,
    const std::vector<std::pair<double, double>>& calibrationPoints,
    const std::shared_ptr<MCCDAQHandler> daq,
    const unsigned int channelConnectedTo)
    : Sensor(deviceID, calibrationPoints), channelConnectedTo{channelConnectedTo}, daq{daq}
{}

void
AnalogMCCDAQSensor::startAcquisition()
{}

void
AnalogMCCDAQSensor::stopAcquisition()
{}

double AnalogMCCDAQSensor::getLatestData() {
    ScanStatus status;
    TransferStatus transferStatus;
    UlError err = ulAInScanStatus(daq->handle, &status, &transferStatus);
    if (err != ERR_NO_ERROR) { LOG(ERROR) << id << ": ulAInScanStatus Error '" << err << "' with MCCDAQ device uniqueID '" << daq->uniqueID << "'"; }

    int connected = 0;
    err = ulIsDaqDeviceConnected(daq->handle, &connected);
    if (err != ERR_NO_ERROR) { LOG(ERROR) << id << ": ulIsDaqDeviceConnected Error: " << err; }

    if (status == SS_RUNNING && connected != 0) {
        // Report average of all samples seen since last run.
        double temp = 0.0;
        for (unsigned int sample = 0; sample < daq->samplesPerChannel; sample++) {
            // This is like a 2D array, databuffer[channel][sample]
            // It may be worth it to try and make 2D arrays work with uldaq but this works so yeah
            temp += daq->dataBuffer[this->channelConnectedTo + (daq->numChannels * sample)];
        }
        this->latestValue = (this->slope * temp / daq->samplesPerChannel) + this->intercept;
    }
    else {
        LOG(ERROR) << "DAQ device ID '" << this->id << "' cannot be accessed. Reporting all NaN.";
    }
    return this->latestValue;
}

#endif
