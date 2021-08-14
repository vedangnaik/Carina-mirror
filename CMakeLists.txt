cmake_minimum_required(VERSION 3.10)
project(Carina VERSION 2.0)

# C++ version
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Show CMake where Qt is
set(CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu")
find_package(Qt5Widgets REQUIRED)
# Enable automatic use of uic, moc, etc.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Get all cpp files from various places and create the target.
file(GLOB_RECURSE ALL_SRCS
    "${PROJECT_SOURCE_DIR}/dependencies/*.cc"
    "${PROJECT_SOURCE_DIR}/dependencies/*.h"
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/*.h"
    "${PROJECT_SOURCE_DIR}/src/*.ui"
    "${PROJECT_SOURCE_DIR}/main.cpp"
)
add_executable(Carina ${ALL_SRCS})

# Target the Qt libraries and Carina's own headers.
target_link_libraries(Carina Qt5::Widgets)
target_include_directories(Carina PUBLIC 
    "${PROJECT_SOURCE_DIR}/dependencies"
    "${PROJECT_SOURCE_DIR}/src/L1/entities"
    "${PROJECT_SOURCE_DIR}/src/L2/usecases"
    "${PROJECT_SOURCE_DIR}/src/L3/controllers"
    "${PROJECT_SOURCE_DIR}/src/L3/gateways"
    "${PROJECT_SOURCE_DIR}/src/L3/presenters"
    "${PROJECT_SOURCE_DIR}/src/L4/control"
    "${PROJECT_SOURCE_DIR}/src/L4/dataacquisition"
    "${PROJECT_SOURCE_DIR}/src/L4/ui"
    "${PROJECT_SOURCE_DIR}/src/L4/ui/DAQManagerConfiguration"
)

# Set maximum warnings
target_compile_options(Carina PRIVATE -Wall -Wextra -pedantic -Werror)