#pragma once

#define OPEN true
#define CLOSED false

#include <string>

class AbstractActuator {
public:
    AbstractActuator(const std::string deviceID);
    virtual void toggle() = 0;
    virtual void setState(const bool state) = 0;
    virtual ~AbstractActuator() {};

    const std::string deviceID;
    bool state = CLOSED; // CLOSED state by default
};
