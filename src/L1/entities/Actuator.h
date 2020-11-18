#ifndef ACTUATOR_H
#define ACTUATOR_H

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
    Actuator(std::string id, std::string name) : id(id), name(name), status(false) {};
    const std::string id;
    const std::string name;
    bool status;
};

#endif // ACTUATOR_H
