#pragma once

#include <gtest/gtest.h>
#include "DummyActuator.h"
#include "ActuatorsManager.h"
#include "ActuatorsPresenter.h"

class ActuatorsManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::unordered_map<std::string, std::unique_ptr<Actuator>> t;
        t.insert({"act1", std::unique_ptr<Actuator>(new DummyActuator("act1"))});
        t.insert({"act2", std::unique_ptr<Actuator>(new DummyActuator("act2"))});
        t.insert({"act3", std::unique_ptr<Actuator>(new DummyActuator("act3"))});
        this->am = std::make_unique<ActuatorsManager>(t, ap);
    }

    ActuatorsPresenter ap;
    std::unique_ptr<ActuatorsManager> am;
};

// Assuming correct initialization, that members, etc. are in the right state.
TEST_F(ActuatorsManagerTest, BasicProperties) {
     this->am->getActuatorStatus("act1");
     this->am->setState("act1", false);
     this->am->getActuatorStatus("act2");
     this->am->setState("act2", false);
     this->am->getActuatorStatus("act3");
     this->am->setState("act3", false);
}

TEST_F(ActuatorsManagerTest, getActuatorStatus_fail) {
    EXPECT_DEATH(this->am->getActuatorStatus("nonExistantID"), "");
    EXPECT_DEATH(this->am->getActuatorStatus("anotherNonExistantID"), "");
}

TEST_F(ActuatorsManagerTest, actuateTest_fail) {
    EXPECT_DEATH(this->am->setState("nonExistantID", false), "");
    EXPECT_DEATH(this->am->setState("anotherNonExistantID", false), "");
}