#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "DummyActuator.h"

class ActuatorTest : public ::testing::Test {
protected:
    DummyActuator a = DummyActuator("act1");
};

TEST_F(ActuatorTest, BasicProperties) {
    ASSERT_EQ(a.id, "act1");
    ASSERT_EQ(a.state, false);
}

TEST_F(ActuatorTest, ChangingStatus) {
    this->a.state = true;
    ASSERT_EQ(this->a.state, true);
    this->a.state = false;
    ASSERT_EQ(this->a.state, false);
}
