include(../common_settings.pri)
include(../dependencies/dependencies.pri)
include(../src/L1/L1.pri)
include(../src/L2/L2.pri)
include(../src/L3/L3.pri)
include(../src/L4/L4.pri)

# For some reason it still compiles even if all these are explicitly removed.
# Included just for completion's sake, best to leave it be.
QT += core gui widgets

SOURCES += $$PWD/main.cpp

# Define the preprocessor variables that activate custom logging to the ErrorUIHandler.
# See dependencies/easylogging++.h to see how these variables are used.
#DEFINES += CUSTOM_COUT_HEADER=\\\"$$PWD/../src/L4/ui/ErrorUIHandler.h\\\"
#DEFINES += CUSTOM_COUT_GLOBAL_VAR=euih
#DEFINES += CUSTOM_COUT_LINE_FORMAT=logLine

# Defines the variable which includes uldaq.h and the associated code for reading real DAQs.
# Turn it on if you want to test real DAQs; it's off by default since uldaq.h is not included
# by default on any Linux distros and cannot be used on Windows at all.
# Note: This doesn't have to be done here, it can also be passed in as an argument to qmake.
# It's just put here because it's the most relevant spot.
#DEFINES += ULDAQ_AVAILABLE
