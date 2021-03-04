// #ifdef WIRINGPI_AVAILABLE

#include "AD799xDAQ.h"

#define BUSY_REG            0x0C
#define ADVANCED_CONFIG_REG 0x0B
#define CONVERSION_RATE_REG 0x07
#define CONFIG_REG          0x00
#define IN0                 0x20

AD799xDAQ::AD799xDAQ(std::string id, unsigned int numChannels, char I2CAddress)
    : AbstractDAQ(id, numChannels), I2CAddress{I2CAddress}
{
    this->fd = -1;
}

void
AD799xDAQ::startAcquisition()
{
    this->fd = wiringPiI2CSetup(I2CAddress);
    if (fd == -1){
        LOG(ERROR) << "Could not setup I2C device at address: " << I2CAddress << std::endl;
    }

    // read the Busy Status Register until the 'Not Ready' bit is 0
    while (wiringPiI2CReadReg8(this->fd, BUSY_REG) != 0x00){
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // program the Advanced Configuration Register
    wiringPiI2CWriteReg8(this->fd, ADVANCED_CONFIG_REG, 0b00000001); // not too sure what bits [2:1] should be

    // program the Conversion Rate Register

    // set limit registers

    // set START big of configuration register to 1
    wiringPiI2CWriteReg8(this->fd, CONFIG_REG, 0b00000001);
}

void
AD799xDAQ::stopAcquisition()
{

}

std::vector<double>
AD799xDAQ::getLatestData()
{
    std::vector<double> values(this->numChannels, std::nan("NaN"));
    if (this->fd != -1) {
        // set up i2c?
        int dataReg1 = IN0;
        for (unsigned int channel = 0; channel < this->numChannels; channel++) {
            // read the 12 bit data from dataReg
            // read values in as 16 bits
            int dataReg = IN0 + channel;
            int val = wiringPiI2CReadReg16(this->fd, dataReg);

            try {
                double& slope = this->slopesAndIntercepts.at(channel).first;
                double& intercept = this->slopesAndIntercepts.at(channel).second;
                values.at(channel) = (slope * std::stod(val)) + intercept; // changed val to sval here
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
