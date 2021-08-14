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
    explicit Actuator(std::string id);
    virtual void setState(bool state) = 0;
    virtual ~Actuator() = default;

    const std::string id;
    bool state = false; // Closed by default
};
