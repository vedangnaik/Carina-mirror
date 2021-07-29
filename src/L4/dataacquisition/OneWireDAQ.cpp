#include "OneWireDAQ.h"

OneWireDAQ::OneWireDAQ(const std::string deviceID, const unsigned int numChannels, std::string devicePath)
    : AbstractDAQ(deviceID, numChannels)
{
    this->devicePath = devicePath;
}

void OneWireDAQ::startAcquisition() {
    // check if file can be opened/exists
}

void OneWireDAQ::stopAcquisition() {

}


std::vector<double> OneWireDAQ::getLatestData()
{
    std::vector<double> values(this->numChannels, std::nan("NaN"));
    if () { // check file exists + openable
        for (unsigned int channel = 0; channel < this->numChannels; channel++) {
            // code to parse the data from /sys/bus/w1/devices/devicePath/w1_slave

            try {
                double& slope = this->slopesAndIntercepts.at(channel).first;
                double& intercept = this->slopesAndIntercepts.at(channel).second;
                values.at(channel) = (slope * val) + intercept; // changed val to sval here @VEDANG
            } catch (std::exception& e) { // fix log statement
                LOG(ERROR) << "DAQ device ID '" << this->deviceID << "': channel value '" << channel << "' could not be read. Reporting NaN.";
            }
        }
    } else {
        LOG(ERROR) << "DAQ device ID '" << this->deviceID << "' cannot be accessed. Reporting all NaN.";
    }
    return values;
}
