#ifdef WIRINGPI_AVAILABLE

#include "SolenoidActuator.h"

SolenoidActuator::SolenoidActuator(std::string deviceID, uint8_t relayChannel, uint8_t gpioPin, bool nominallyPowered)
    : Actuator(std::move(deviceID)), gpioPin{gpioPin}, relayChannel{relayChannel}, nominallyPowered{nominallyPowered}
{}

void SolenoidActuator::setState(const bool state) {
    /* four bools to keep track of:
     * - state on or off
     * - relay connection nominally closed or open
     * - gpio pin high or low (whether we want to turn relay off or on respectively)
     * - solenoid nominally open or closed
    */
    this->state = state;
    if (this->state) {  //  this->state == this->nominallyPowered
        LOG(INFO) << "Solenoid '" << this->id << "' at relay channel " << std::to_string(this->relayChannel) << " is now turned ON.";  // OFF
    } else {
        LOG(INFO) << "Solenoid '" << this->id << "' at relay channel " << std::to_string(this->relayChannel) << " is now turned OFF.";  // ON
    }

    setDigitalPin(this->gpioPin, this->state);
}

void SolenoidActuator::setDigitalPin(uint8_t pin, bool turnRelayChannelOn) {
    // https://www.digikey.ca/en/maker/blogs/2019/how-to-use-gpio-on-the-raspberry-pi-with-c
    // pinMode(pin, OUTPUT);
    std::string s1 = "gpio -g mode ";
    std::string s2 = "gpio -g write ";
    
    std::string output = " output";
    std::string high = " 1";
    std::string low = " 0";

    std::string s_set_mode = s1 + std::to_string(pin) + output;
    const char * set_mode = s_set_mode.c_str();
    
    std::string s_set_high = s2 + std::to_string(pin) + high;
    const char * set_high = s_set_high.c_str();
    
    std::string s_set_low = s2 + std::to_string(pin) + low;
    const char * set_low = s_set_low.c_str();
    
    system(set_mode);

    if (turnRelayChannelOn) {
        // digitalWrite(pin, 0);  // yes, activating the channel means pulling it to ground
        system(set_low);
    } else {
        // digitalWrite(pin, 1);
        system(set_high);
    }
}

#endif
