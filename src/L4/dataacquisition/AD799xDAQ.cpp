// #ifdef WIRINGPI_AVAILABLE

#include "AD799xDAQ.h"

AD799xDAQ::AD799xDAQ(std::string id, unsigned int numChannels, char I2CAddress, std::string I2CPath)
    : AbstractDAQ(id, numChannels), I2CAddress{I2CAddress}, I2CPath({I2CPath})
{
    std::ifstream test(I2CPath);
    if (!test.is_open()) {
        LOG(ERROR) << "I2CPortHandler error - device id: " << this->deviceID << ", num channels: " << this->numChannels << ", I2C port path: " << this->I2CPath << "; Serial port path could not be opened.";
        // throw exception here
    }
}

void
AD799xDAQ::startAcquisition()
{

}

void
AD799xDAQ::stopAcquisition()
{

}

std::vector<double>
AD799xDAQ::getLatestData()
{
    std::vector<double> values(this->numChannels, std::nan("NaN"));
    if (this->I2CPort.is_open()) {
        // set up i2c?

        for (unsigned int channel = 0; channel < this->numChannels; channel++) {
            // add code to read the values
            std::string sval;

            try {
                double& slope = this->slopesAndIntercepts.at(channel).first;
                double& intercept = this->slopesAndIntercepts.at(channel).second;
                values.at(channel) = (slope * std::stod(sval)) + intercept;
            } catch (std::exception& e) {
                LOG(ERROR) << "DAQ device ID '" << this->deviceID << "': channel value '" << channel << "' could not be read. Reporting NaN.";
            }
        }
    } else {
        LOG(ERROR) << "DAQ device ID '" << this->deviceID << "' cannot be accessed. Reporting all NaN.";
    }
    return values;
}

// #endif
