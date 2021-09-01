cmake_minimum_required(VERSION 3.14)
project(my_project)

# GoogleTest requires at least C++11.
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

## Show CMake where Qt is.
set(CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu")
find_package(Qt5Widgets REQUIRED)
# Enable automatic use of uic, moc, etc.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Grab all the cpp, h, and ui files from various locations.
# We first use GLOB_RECURSE to get all the files from src, since they're nested quite deep.
file(GLOB_RECURSE ALL_SRCS
    "${PROJECT_SOURCE_DIR}/../src/*.cpp"
    "${PROJECT_SOURCE_DIR}/../src/*.h"
    "${PROJECT_SOURCE_DIR}/../src/*.ui"
    "${PROJECT_SOURCE_DIR}/*.h"
)
# We then add on the singular files in dependencies and main.cpp.
set(ALL_SRCS ${ALL_SRCS}
    "${PROJECT_SOURCE_DIR}/../dependencies/easyloggingpp/src/easylogging++.cc"
    "${PROJECT_SOURCE_DIR}/../dependencies/easyloggingpp/src/easylogging++.h"
    "${PROJECT_SOURCE_DIR}/../dependencies/Helpers.cpp"
    "${PROJECT_SOURCE_DIR}/../dependencies/Helpers.h"
    "${PROJECT_SOURCE_DIR}/../dependencies/Adafruit_PWMServoDriver.cpp"
    "${PROJECT_SOURCE_DIR}/../dependencies/Adafruit_PWMServoDriver.h"
    "${PROJECT_SOURCE_DIR}/tests_main.cpp"
)

add_executable(tests ${ALL_SRCS})
target_link_libraries(tests Qt5::Widgets)
target_link_libraries(tests gtest_main)

target_include_directories(tests PUBLIC
    "${PROJECT_SOURCE_DIR}/../dependencies"
    "${PROJECT_SOURCE_DIR}/../dependencies/easyloggingpp/src"
    "${PROJECT_SOURCE_DIR}/../dependencies/googletest/googletest"
    "${PROJECT_SOURCE_DIR}/../dependencies/googletest/googletest/include"
    "${PROJECT_SOURCE_DIR}/../dependencies/googletest/googlemock"
    "${PROJECT_SOURCE_DIR}/../dependencies/googletest/googlemock/include"
    "${PROJECT_SOURCE_DIR}/../src/L1/entities"
    "${PROJECT_SOURCE_DIR}/../src/L2/usecases"
    "${PROJECT_SOURCE_DIR}/../src/L3/controllers"
    "${PROJECT_SOURCE_DIR}/../src/L3/gateways"
    "${PROJECT_SOURCE_DIR}/../src/L3/presenters"
    "${PROJECT_SOURCE_DIR}/../src/L4/control"
    "${PROJECT_SOURCE_DIR}/../src/L4/dataacquisition"
    "${PROJECT_SOURCE_DIR}/../src/L4/ui"
    "${PROJECT_SOURCE_DIR}"
)

include(GoogleTest)
gtest_discover_tests(tests)