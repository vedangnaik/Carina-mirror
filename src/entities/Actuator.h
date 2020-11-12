#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <string>

enum ActuatorOption {
    Timed,
    Automatic
};

enum ActuatorCheck {
    Close = 0,
    Open = 1
};

class Actuator {
public:
    Actuator(std::string id, std::string name);
    std::string id;
    std::string name;
    bool status;
};

#endif // ACTUATOR_H
