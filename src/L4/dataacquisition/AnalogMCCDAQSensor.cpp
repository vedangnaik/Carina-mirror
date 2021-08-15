 #ifdef ULDAQ_AVAILABLE

 #include "AnalogMCCDAQSensor.h"

 AnalogMCCDAQSensor::AnalogMCCDAQSensor(const std::string& deviceID,
                                        const std::array<std::pair<double, double>, 5>& calibrationPoints,
                                        const unsigned int channelConnectedTo,
                                        DaqDeviceHandle handle)
     : Sensor(deviceID, calibrationPoints), handle{handle}, channelConnectedTo{channelConnectedTo}
 {
     // Get number of channels
     err = ulAIGetInfo(handle, AI_INFO_NUM_CHANS_BY_MODE, AI_SINGLE_ENDED, this->numChannels);
     if (err != ERR_NO_ERROR) { std::cout << "ulAIGetInfo Error: " << err << std::endl; }

     // allocate the temporary data buffer
     this->dataBuffer = std::make_unique<double[]>(this->numChannels * this->samplesPerChannel * sizeof(double));
 }

 void AnalogMCCDAQSensor::startAcquisition() {
     UlError err = ulAInScan(this->handle, 0, this->numChannels-1, this->aiim, this->voltageRange, this->samplesPerChannel, &this->rate, this->so, this->aisf, this->dataBuffer.get());
     if (err != ERR_NO_ERROR) { LOG(ERROR) << "ulAInScan Error: " << err; }
 }

 void AnalogMCCDAQSensor::stopAcquisition() {
     UlError err = ulAInScanStop(this->handle);
     if (err != ERR_NO_ERROR) { LOG(ERROR) << "ulAInScanStop Error: " << err; }
 }

 double AnalogMCCDAQSensor::getLatestData() {
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
         double temp = 0.0;
         for (unsigned int sample = 0; sample < this->samplesPerChannel; sample++) {
             // This is like a 2D array, databuffer[channel][sample]
             // It may be worth it to try and make 2D arrays work with uldaq but this works so yeah
             temp += this->dataBuffer[this.channelConnectedTo + (this->numChannels * sample)];
         }
         this->latestValue = (this->slope * temp / this->samplesPerChannel) + this->intercept;
     } else {
         LOG(ERROR) << "DAQ device ID '" << this->id << "' cannot be accessed. Reporting all NaN.";
     }
     return this->latestValue;
 }

 #endif
