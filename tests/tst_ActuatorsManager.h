#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "ActuatorsManager.h"

class ActuatorsManagerTest : public ::testing::Test {
protected:
    std::map<std::string, Actuator*> testActuators;
    ActuatorsManager am = ActuatorsManager({
       {"act1", Actuator("act1", "act1name")},
       {"act2", Actuator("act2", "act2name")},
       {"act3", Actuator("act3", "act3name")}
   });
};

// Assuming correct initialization, that members, etc. are in the right state.
TEST_F(ActuatorsManagerTest, BasicProperties) {
    for (const auto& p : this->testActuators) {
        // Do nothing here: if it crashes, it "failed" the test :D
        this->am.getActuatorStatus(p.first);
        this->am.actuate(p.first);
    }
}

TEST_F(ActuatorsManagerTest, getActuatorStatus_fail) {
    EXPECT_DEATH(this->am.getActuatorStatus("nonExistantID"), "");
    EXPECT_DEATH(this->am.getActuatorStatus("anotherNonExistantID"), "");
}

TEST_F(ActuatorsManagerTest, actuateTest_fail) {
    EXPECT_DEATH(this->am.actuate("nonExistantID"), "");
    EXPECT_DEATH(this->am.actuate("anotherNonExistantID"), "");
}
