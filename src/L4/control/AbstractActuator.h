#pragma once

#include <string>

class AbstractActuator {
public:
    AbstractActuator(const std::string deviceID);
    virtual void toggle(const bool state) = 0;
    virtual ~AbstractActuator() {};

    const std::string deviceID;
    const bool state;
};
