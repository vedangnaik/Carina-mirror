#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "Actuator.h"

class ActuatorTest : public ::testing::Test {
protected:
    Actuator a = Actuator("act1", "act1name");
};

TEST_F(ActuatorTest, BasicProperties) {
    ASSERT_EQ(a.id, "act1");
    EXPECT_EQ(a.name, "act1name");
    ASSERT_EQ(a.status, false);
}

TEST_F(ActuatorTest, ChangingStatus) {
    this->a.status = true;
    ASSERT_EQ(this->a.status, true);
    this->a.status = false;
    ASSERT_EQ(this->a.status, false);
}
