#include <gtest/gtest.h>
// Test headers
#include "tests/tst_Actuator.h"
#include "tests/tst_ActuatorsManager.h"
#include "tests/tst_Sensor.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
