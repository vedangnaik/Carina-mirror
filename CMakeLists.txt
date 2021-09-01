###
# 1. Basics - compiler version, CMake version, yada yada.
###
cmake_minimum_required(VERSION 3.10)
project(Carina VERSION 2.0)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)



###
# 2. Set up Qt and it's custom compilation steps.
###
set(CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu")
find_package(Qt5Widgets REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)



###
# 3. Fetch easyloggingpp.
###
include(FetchContent)
FetchContent_Declare(
        easyloggingpp
        GIT_REPOSITORY https://github.com/amrayn/easyloggingpp.git
)
FetchContent_MakeAvailable(easyloggingpp) # At this point, the variable easyloggingpp_SOURCE_DIR points to the base of the easyloggingpp repository.



###
# 4. Set up variables for conditional compilation of uldaq and Raspberry Pi-specific files.
###
if (ULDAQ_AVAILABLE)
    message("Compiling with MCCDAQ Uldaq support...")
    add_definitions(-DULDAQ_AVAILABLE=1)
endif()
if (WIRINGPI_AVAILABLE)
    message("Compiling with WiringPi support...")
    add_definitions(-DWIRINGPI_AVAILABLE=1)
endif()



###
# 5. Define source files common to both Carina and it's unit tests.
###
# First, the src/ folder. We use GLOB_RECURSE to get all the files since they're nested quite deep.
file(GLOB_RECURSE ALL_SRCS
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/*.h"
    "${PROJECT_SOURCE_DIR}/src/*.ui"
)
# Next, specific dependencies and files.
set(ALL_SRCS ${ALL_SRCS}
    "${PROJECT_SOURCE_DIR}/dependencies/Adafruit_PWMServoDriver.cpp"
    "${PROJECT_SOURCE_DIR}/dependencies/Adafruit_PWMServoDriver.h"
    "${PROJECT_SOURCE_DIR}/dependencies/Helpers.cpp"
    "${PROJECT_SOURCE_DIR}/dependencies/Helpers.h"
    "${easyloggingpp_SOURCE_DIR}/src/easylogging++.cc"
    "${easyloggingpp_SOURCE_DIR}/src/easylogging++.h"
)



###
# 6. Set up the Carina target - link appropriate libraries, create include path, and any compiler arguments.
###
# Add Carina's main.cpp here first.
set(ALL_SRCS ${ALL_SRCS} "${PROJECT_SOURCE_DIR}/main.cpp")
add_executable(Carina ${ALL_SRCS})

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
target_include_directories(Carina PUBLIC
    "${PROJECT_SOURCE_DIR}/dependencies"
    "${easyloggingpp_SOURCE_DIR}/src"
    "${PROJECT_SOURCE_DIR}/src/L1/entities"
    "${PROJECT_SOURCE_DIR}/src/L2/usecases"
    "${PROJECT_SOURCE_DIR}/src/L3/controllers"
    "${PROJECT_SOURCE_DIR}/src/L3/gateways"
    "${PROJECT_SOURCE_DIR}/src/L3/presenters"
    "${PROJECT_SOURCE_DIR}/src/L4/control"
    "${PROJECT_SOURCE_DIR}/src/L4/dataacquisition"
    "${PROJECT_SOURCE_DIR}/src/L4/ui"
)

# Compiler options - set up maximum warnings.
# TODO: Perhaps call UbSAN and ASAN here as well.
target_compile_options(Carina PRIVATE -Wall -Wextra -pedantic)