 #ifdef ULDAQ_AVAILABLE
 #pragma once

 #include "Sensor.h"
 #include "easylogging++.h"
 #include <uldaq.h>
 #include <cstdlib>
 #include <map>
 #include <vector>
 #include <memory>
 #include <iostream>
 #include <cmath>

 /**
  * @brief The AiDAQHandler class handles an analog input DAQ device
  */
 class AnalogMCCDAQSensor : public Sensor {
 public:
     AnalogMCCDAQSensor(const std::string& deviceID,
                        const std::vector<std::pair<double, double>>& calibrationPoints,
                        unsigned int channelConnectedTo,
                        DaqDeviceHandle handle);
     void startAcquisition() override;
     double getLatestData() override;
     void stopAcquisition() override;
 private:
     // config vars
     const DaqDeviceHandle handle;
     const unsigned int channelConnectedTo;
     long long numChannels;
     const unsigned int samplesPerChannel = 100;
     Range voltageRange;
     double rate = 100; // must be non-const for uldaq.h :(
     const AiInputMode aiim = AI_SINGLE_ENDED;
     const ScanOption so = SO_CONTINUOUS;
     const AInScanFlag aisf = AINSCAN_FF_DEFAULT;
     // status vars
     std::unique_ptr<double[]> dataBuffer;
 };

 #endif
