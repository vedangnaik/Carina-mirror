#include <gtest/gtest.h>
#include "easylogging++.h"
// Test headers
#include "tst_Actuator.h"
#include "tst_ActuatorsManager.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
