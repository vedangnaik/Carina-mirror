#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "DummySensor.h"

class SensorTest : public ::testing::Test {
protected:
    const std::vector<std::pair<double, double>> t {
        {1.0, 1.0},
        {2.0, 2.0}
    };
    DummySensor s = DummySensor("sens1", t);
};

TEST_F(SensorTest, BasicProperties) {
    ASSERT_EQ(s.id, "sens1");
    ASSERT_EQ(s.latestValue, 0.0);
}
