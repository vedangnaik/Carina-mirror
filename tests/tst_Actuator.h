#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "Actuator.h"

class ActuatorTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::string id = "act1";
        std::string name = "act1name";
        this->a = std::make_unique<Actuator>(id, name);
        ASSERT_NE(this->a, nullptr);
    }

    std::unique_ptr<Actuator> a;
};

TEST_F(ActuatorTest, BaseProperties) {
    EXPECT_EQ(a->id, "act1");
    EXPECT_EQ(a->name, "act1name");
    EXPECT_EQ(a->status, false);
}

TEST_F(ActuatorTest, ChangingStatus) {
    this->a->status = true;
    EXPECT_EQ(this->a->status, true);
    this->a->status = false;
    EXPECT_EQ(this->a->status, false);
}
