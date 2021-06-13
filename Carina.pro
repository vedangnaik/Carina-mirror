include(common_settings.pri)
include(dependencies/dependencies.pri)
include(src/L1/L1.pri)
include(src/L2/L2.pri)
include(src/L3/L3.pri)
include(src/L4/L4.pri)

# For some reason it still compiles even if all these are explicitly removed.
# Included just for completion's sake, best to leave it be.
QT += core gui widgets

SOURCES += carina_main.cpp
