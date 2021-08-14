#pragma once

#include "Actuator.h"
#include "easylogging++.h"

class DummyActuator : Actuator {
public:
    DummyActuator(const std::string id);
    void setState(const bool state) override;
};