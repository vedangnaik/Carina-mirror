#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "ActuatorsManager.h"

class ActuatorsManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        this->act1 = std::make_unique<Actuator>("act1", "act1name");
        this->act2 = std::make_unique<Actuator>("act2", "act2name");
        this->act3 = std::make_unique<Actuator>("act3", "act3name");
        this->testActuators = {
            {"act1", this->act1.get()},
            {"act2", this->act2.get()},
            {"act3", this->act3.get()}
        };

        // Constructor shouldn't crash with these correct inputs.
        ASSERT_NO_THROW(this->am = std::make_unique<ActuatorsManager>(testActuators));
        ASSERT_NE(this->am, nullptr);
    }

    std::unique_ptr<Actuator> act1;
    std::unique_ptr<Actuator> act2;
    std::unique_ptr<Actuator> act3;
    std::map<std::string, Actuator*> testActuators;
    std::unique_ptr<ActuatorsManager> am;
};

// Assuming correct initialization, that members, etc. are in the right state.
TEST_F(ActuatorsManagerTest, BasicProperties) {
    for (const auto& [id, _] : this->testActuators) {
        // Do nothing here: if it crashes, it "failed" the test :D
        this->am->getActuatorStatus(id);
        this->am->actuate(id);
    }
}

TEST_F(ActuatorsManagerTest, getActuatorStatus_fail) {
    EXPECT_DEATH(this->am->getActuatorStatus("nonExistantID"), "");
    EXPECT_DEATH(this->am->getActuatorStatus("anotherNonExistantID"), "");
}

TEST_F(ActuatorsManagerTest, actuateTest_fail) {
    EXPECT_DEATH(this->am->actuate("nonExistantID"), "");
    EXPECT_DEATH(this->am->actuate("anotherNonExistantID"), "");
}
