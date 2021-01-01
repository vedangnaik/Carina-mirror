#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "ActuatorsManager.h"

using namespace testing;

TEST(ActuatorsManager, BasicProperties)
{
    EXPECT_EQ(1, 1);
    ASSERT_THAT(0, Eq(0));
    auto am = new ActuatorsManager({});
}

