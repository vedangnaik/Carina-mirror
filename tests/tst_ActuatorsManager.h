#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "ActuatorsManager.h"
#include "ActuatorsPresenter.h"

class ActuatorsManagerTest : public ::testing::Test {
protected:
    std::vector<std::string> testActuatorIDs = {"act1", "act2", "act3"};
    ActuatorsPresenter ap = ActuatorsPresenter();
    ActuatorsManager am = ActuatorsManager({
       {"act1", Actuator("act1", "act1name")},
       {"act2", Actuator("act2", "act2name")},
       {"act3", Actuator("act3", "act3name")}
   }, ap);
};

// Assuming correct initialization, that members, etc. are in the right state.
TEST_F(ActuatorsManagerTest, BasicProperties) {
    for (const auto& id : this->testActuatorIDs) {
        // Do nothing here: if it crashes, it "failed" the test :D
        this->am.getActuatorStatus(id);
        this->am.actuate(id);
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
