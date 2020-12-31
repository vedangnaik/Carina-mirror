#include <gtest/gtest.h>
// Test headers
#include "test_Actuator.h"
#include "test_ActuatorsManager.h"

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
