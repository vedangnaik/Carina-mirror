#pragma once

#include "Actuator.h"
#include "easylogging++.h"

class DummyActuator : public Actuator {
public:
    explicit DummyActuator(const std::string& id);
    void setState(bool state) override;
};