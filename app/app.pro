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
DEFINES += CUSTOM_COUT_HEADER=\\\"$$PWD/../src/L4/ui/ErrorUIHandler.h\\\"
DEFINES += CUSTOM_COUT_GLOBAL_VAR=euih
DEFINES += CUSTOM_COUT_LINE_FORMAT=logLine
