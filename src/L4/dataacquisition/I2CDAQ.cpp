#ifdef WIRINGPI_AVAILABLE

#include "I2CDAQ.h"

#define BUSY_REG            0x0C
#define ADVANCED_CONFIG_REG 0x0B
#define CONVERSION_RATE_REG 0x07
#define CONFIG_REG          0x00
#define IN0                 0x20
#define DEEP_SHUTDOWN_REG   0x0A
#define INTERUPT_MASK_REG   0x03
#define CONVERSION_RATE_REG 0x07

I2CDAQ::I2CDAQ(const std::string id, const unsigned int numChannels, const char I2CAddress)
    : Sensor(id, numChannels), I2CAddress{I2CAddress}
{
    this->fd = -1;
}

void
I2CDAQ::startAcquisition()
{
    int result;
    this->fd = wiringPiI2CSetup(I2CAddress);
    if (fd == -1){
        LOG(ERROR) << "Could not setup I2C device at address: " << I2CAddress << std::endl;
    }

    // read the Busy Status Register until the 'Not Ready' bit is 0
    while (wiringPiI2CReadReg8(this->fd, BUSY_REG) != 0x00){
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // program the Advanced Configuration Register
    result = wiringPiI2CWriteReg8(this->fd, ADVANCED_CONFIG_REG, 0x01);
    if (result == -1){
        LOG(ERROR) << "Failed to write to the Advanced Configuration Register I2CDAQ::startAcquisition() for the device at: " << I2CAddress << std::endl;
        // throw exception?

    }

    // program the Conversion Rate Register to continuous conversion
    result = wiringPiI2CWriteReg8(this->fd, CONVERSION_RATE_REG, 0x01);
    if (result == -1){
        LOG(ERROR) << "Failed to write to the Conversion Rate Register I2CDAQ::startAcquisition() for the device at: " << I2CAddress << std::endl;
        // throw exception?

    }

    // mask interrupts
    result = wiringPiI2CWriteReg8(this->fd, INTERUPT_MASK_REG, 0xFF);
    if (result == -1){
        LOG(ERROR) << "Failed to write to the Interupt Mask Register I2CDAQ::startAcquisition() for the device at: " << I2CAddress << std::endl;
        // throw exception?

    }

    // set START bit of configuration register to 1
    result = wiringPiI2CWriteReg8(this->fd, CONFIG_REG, 0x01);
    if (result == -1){
        LOG(ERROR) << "Failed to write to the Configuration Register in I2CDAQ::startAcquisition() for the device at: " << I2CAddress << std::endl;
        // throw exception?

    }
}

void
I2CDAQ::stopAcquisition()
{
    int result;
    // set start bit to 0
    result = wiringPiI2CWriteReg8(this->fd, CONFIG_REG, 0x00);
    if (result == -1){
        LOG(ERROR) << "Failed to write to the Configuration Register in I2CDAQ::stopAcquisition() for the device at: " << I2CAddress << std::endl;
        // throw exception?

    }
}

std::vector<double>
I2CDAQ::getLatestData()
{
    std::vector<double> values(this->numChannels, std::nan("NaN"));
    if (this->fd != -1) {
        int dataReg1 = IN0;
        for (unsigned int channel = 0; channel < this->numChannels; channel++) {
            // read the 12 bit data from dataReg
            // read values in as 16 bits
            int dataReg = IN0 + channel;
            int val = wiringPiI2CReadReg16(this->fd, dataReg);

            try {
                double& slope = this->slopesAndIntercepts.at(channel).first;
                double& intercept = this->slopesAndIntercepts.at(channel).second;
                values.at(channel) = (slope * val) + intercept; // changed val to sval here @VEDANG
            } catch (std::exception& e) {
                LOG(ERROR) << "DAQ device ID '" << this->deviceID << "': channel value '" << channel << "' could not be read. Reporting NaN.";
            }
        }
    } else {
        LOG(ERROR) << "DAQ device ID '" << this->deviceID << "' cannot be accessed. Reporting all NaN.";
    }
    return values;
}

#endif
