###
# 1. Basics - compiler version, CMake version, yada yada.
###
cmake_minimum_required(VERSION 3.15)
project(Carina VERSION 2.0)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)



###
# 2. Set up Qt and its custom compilation steps.
###
set(CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu")
find_package(Qt5Widgets REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)



###
# 4. Set up variables for conditional compilation of uldaq, Raspberry Pi-specific files, and unit tests.
###
if (ULDAQ_AVAILABLE)
    message("Compiling with MCCDAQ Uldaq support...")
    add_definitions(-DULDAQ_AVAILABLE=1)
endif()
if (WIRINGPI_AVAILABLE)
    message("Compiling with WiringPi support...")
    add_definitions(-DWIRINGPI_AVAILABLE=1)
endif()
if (GOOGLETEST_REQUIRED)
    message("Compiling unit tests with googletest...")
    add_definitions(-DGOOGLETEST_REQUIRED=1)
endif()



###
# 5. Define source files common to both Carina and it's unit tests.
###
# First, the src/ folder. We use GLOB_RECURSE to get all the files since they're nested quite deep.
file(GLOB_RECURSE COMMON_SRCS
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/*.h"
    "${PROJECT_SOURCE_DIR}/src/*.ui"
)
# Next, specific dependencies and files.
set(COMMON_SRCS ${COMMON_SRCS}
    "${PROJECT_SOURCE_DIR}/dependencies/Adafruit_PWMServoDriver.cpp"
    "${PROJECT_SOURCE_DIR}/dependencies/Adafruit_PWMServoDriver.h"
    "${PROJECT_SOURCE_DIR}/dependencies/Helpers.cpp"
    "${PROJECT_SOURCE_DIR}/dependencies/Helpers.h"
    "${PROJECT_SOURCE_DIR}/dependencies/easylogging++.cc"
    "${PROJECT_SOURCE_DIR}/dependencies/easylogging++.h"
)
# Finally, all common include paths
set(COMMON_INCLUDES
    "${PROJECT_SOURCE_DIR}/dependencies"
    "${PROJECT_SOURCE_DIR}/src/L1/entities"
    "${PROJECT_SOURCE_DIR}/src/L2/usecases"
    "${PROJECT_SOURCE_DIR}/src/L3/controllers"
    "${PROJECT_SOURCE_DIR}/src/L3/gateways"
    "${PROJECT_SOURCE_DIR}/src/L3/presenters"
    "${PROJECT_SOURCE_DIR}/src/L4/control"
    "${PROJECT_SOURCE_DIR}/src/L4/dataacquisition"
    "${PROJECT_SOURCE_DIR}/src/L4/ui"
)



###
# 6. Set up the Carina target - link appropriate libraries, create include path, and any compiler arguments.
###
# Add Carina's main.cpp here first.
add_executable(Carina
    ${COMMON_SRCS}
    "${PROJECT_SOURCE_DIR}/main.cpp"
)

# Link Qt.
target_link_libraries(Carina Qt5::Widgets)
# Link uldaq if needed.
if (ULDAQ_AVAILABLE)
    target_link_libraries(Carina uldaq)
endif()
# Link wiringPi if needed.
if (WIRINGPI_AVAILABLE)
    target_link_libraries(Carina wiringPi)
endif()

# Include paths.
target_include_directories(Carina PUBLIC ${COMMON_INCLUDES})

# Compiler options - set up maximum warnings.
# TODO: Perhaps call UbSAN and ASAN here as well.
target_compile_options(Carina PRIVATE -Wall -Wextra -pedantic)



###
# 7. Download and set up googletest.
###
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)



###
# 8. Set up tests if required.
###
if (GOOGLETEST_REQUIRED)
    # Add googletest and testing headers to list of sources.
    add_executable(tests
        ${COMMON_SRCS}
        "${PROJECT_SOURCE_DIR}/tests/tests_main.cpp"
        "${PROJECT_SOURCE_DIR}/tests/tst_ActuatorsManager.h"
        "${PROJECT_SOURCE_DIR}/tests/tst_Actuator.h"
        "${PROJECT_SOURCE_DIR}/tests/tst_Sensor.h"
    )

    # Link Qt and googletest.
    target_link_libraries(tests Qt5::Widgets gtest_main)
    # Link uldaq if needed.
    if (ULDAQ_AVAILABLE)
        target_link_libraries(tests uldaq)
    endif()
    # Link wiringPi if needed.
    if (WIRINGPI_AVAILABLE)
        target_link_libraries(tests wiringPi)
    endif()

    # Add include paths.
    target_include_directories(tests PUBLIC
        ${COMMON_INCLUDES}
        "${googletest_SOURCE_DIR}/googletest"
        "${googletest_SOURCE_DIR}/googletest/include"
        "${googletest_SOURCE_DIR}/googlemock"
        "${googletest_SOURCE_DIR}/googlemock/include"
    )
endif()