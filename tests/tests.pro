# Force Qt version 5 or higher.
lessThan(QT_MAJOR_VERSION, 5): error("Qt verion 5 or greater needed.")
# Use c++14.
CONFIG += c++14
# Maximum compiler warnings.
CONFIG += warn_on
QMAKE_CXXFLAGS += -pedantic
# Enable warnings if deprecated Qt libraries are used.
DEFINES += QT_DEPRECATED_WARNINGS


include(../dependencies/dependencies.pri)
include(../src/L1/L1.pri)
include(../src/L2/L2.pri)
include(../src/L3/L3.pri)

QT += core gui widgets

SOURCES += \
    tests_main.cpp \
    ../../googletest/googletest/src/gtest-all.cc \
    ../../googletest/googlemock/src/gmock-all.cc \


# The headers appear to require the name "tst_..." to show up in Qt Creator's testing pane.
# Best to just stick to that pattern for now.
HEADERS += \
    tst_ActuatorsManager.h \
    tst_Actuator.h \
    tst_Sensor.h

INCLUDEPATH += \
    ../../googletest/googletest \
    ../../googletest/googletest/include \
    ../../googletest/googlemock \
    ../../googletest/googlemock/include
