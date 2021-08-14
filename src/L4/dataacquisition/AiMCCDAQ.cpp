// #ifdef ULDAQ_AVAILABLE

// #include "AiMCCDAQ.h"

// AiMCCDAQ::AiMCCDAQ(const std::string deviceID, const unsigned int numChannels, DaqDeviceHandle handle, Range voltageRange)
//     : Sensor(deviceID, numChannels), handle{handle}, voltageRange{voltageRange}
// {
//     // connect DAQ
//     UlError err = ulConnectDaqDevice(handle);
//     if (err != ERR_NO_ERROR) { LOG(ERROR) << "ulConnectDaqDevice Error: " << err; }

//     // allocate the temporary data buffer
//     this->dataBuffer = std::make_unique<double[]>(this->numChannels * this->samplesPerChannel * sizeof(double));
// }

// AiMCCDAQ::~AiMCCDAQ() {
//     this->stopAcquisition();
// }

// void AiMCCDAQ::startAcquisition() {
//     UlError err = ulAInScan(this->handle, 0, this->numChannels-1, this->aiim, this->voltageRange, this->samplesPerChannel, &this->rate, this->so, this->aisf, this->dataBuffer.get());
//     if (err != ERR_NO_ERROR) { LOG(ERROR) << "ulAInScan Error: " << err; }
// }

// void AiMCCDAQ::stopAcquisition() {
//     UlError err = ulAInScanStop(this->handle);
//     if (err != ERR_NO_ERROR) { LOG(ERROR) << "ulAInScanStop Error: " << err; }
// }

// std::vector<double> AiMCCDAQ::getLatestData() {
//     ScanStatus status;
//     TransferStatus transferStatus;
//     UlError err = ulAInScanStatus(this->handle, &status, &transferStatus);
//     if (err != ERR_NO_ERROR) { LOG(ERROR) << "ulAInScanStatus Error: " << err; }

//     int connected = 0;
//     err = ulIsDaqDeviceConnected(this->handle, &connected);
//     if (err != ERR_NO_ERROR) { LOG(ERROR) << "ulIsDaqDeviceConnected Error: " << err; }

//     std::vector<double> values(this->numChannels, std::nan("NaN"));
//     if (status == SS_RUNNING && connected != 0) {
//         // Report average of all samples seen since last run.
//         for (unsigned int channel = 0; channel < this->numChannels; channel++) {
//             double& slope = this->slopesAndIntercepts.at(channel).first;
//             double& intercept = this->slopesAndIntercepts.at(channel).second;
//             double temp = 0.0;
//             for (unsigned int sample = 0; sample < this->samplesPerChannel; sample++) {
//                 // This is like a 2D array, databuffer[channel][sample]
//                 // It may be worth it to try and make 2D arrays work with uldaq but this works so yeah
//                 temp += this->dataBuffer[channel + (this->numChannels * sample)];
//             }
//             values[channel] = (slope * temp / this->samplesPerChannel) + intercept;
//         }
//     } else {
//         LOG(ERROR) << "DAQ device ID '" << this->deviceID << "' cannot be accessed. Reporting all NaN.";
//     }
//     return values;
// }

// #endif
