#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "../src/L1/entities/Actuator.h"

TEST(ActuatorTest, BasicProperies) {
    std::string id = "act1";
    std::string name = "act1name";
    Actuator a(id, name);
    ASSERT_EQ(a.id, id) << "Unequal actuator ID: '" << a.id << "' vs. '" << id << "'.";
    ASSERT_EQ(a.name, name) << "Unequal actuator name: '" << a.name << "' vs. '" << name << "'.";
}
