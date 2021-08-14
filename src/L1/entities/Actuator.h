#pragma once

#include <string>

enum class ActuatorOption {
    Timed,
    Automatic
};

struct ActuatorCheck {
    bool status;
};

class Actuator {
public:
    Actuator(const std::string id);
    virtual void setState(const bool state) = 0;
    virtual ~Actuator() {};

    const std::string id;
    bool state = false; // Closed by default
};
