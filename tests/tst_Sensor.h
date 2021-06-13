#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "Sensor.h"

class SensorTest : public ::testing::Test {
protected:
    Sensor s = Sensor("sens1", "sens1name");
};

TEST_F(SensorTest, BasicProperties) {
    ASSERT_EQ(s.id, "sens1");
    ASSERT_EQ(s.name, "sens1name");
    ASSERT_EQ(s.values.at(0), 0.0);
    ASSERT_EQ(s.values.size(), 1);
}
