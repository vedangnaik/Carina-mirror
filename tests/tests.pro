# gtest_dependency.pri was created by Qt Creator when this googletest automated test projet was created.
# It probably needs to be replaced with something more cross-platform for general use.
include(gtest_dependency.pri)
include(../common_settings.pri)
include(../dependencies/dependencies.pri)
include(../src/L1/L1.pri)
include(../src/L2/L2.pri)

# These variables were automatically added by Qt Creator as well.
# Presumably they have something to do with the tests showing up in the testing pane?
# I couldn't find any documentation regarding the values for CONFIG at least...
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt

QT += core gui widgets

SOURCES += $$PWD/main.cpp

# The headers appear to require the name "tst_..." to show up in Qt Creator's testing pane.
# Best to just stick to that pattern for now.
HEADERS += \
    $$PWD/tst_ActuatorsManager.h \
    tst_Actuator.h

# Disable easylogging++, it's not necessary for testing.
DEFINES += ELPP_DISABLE_LOGS
