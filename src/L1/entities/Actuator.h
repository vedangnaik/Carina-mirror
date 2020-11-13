#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <string>

enum class ActuatorOption {
    None,
    Timed,
    Automatic
};

struct ActuatorCheck {
    bool status;
};

class Actuator {
public:
    Actuator(std::string id, std::string name);
    std::string id;
    std::string name;
    bool status;
};

#endif // ACTUATOR_H
