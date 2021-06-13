# Force Qt version 5 or higher.
lessThan(QT_MAJOR_VERSION, 5): error("Qt verion 5 or greater needed.")
# Use c++14.
CONFIG += c++14
# Maximum compiler warnings.
CONFIG += warn_on
QMAKE_CXXFLAGS += -pedantic
# Enable warnings if deprecated Qt libraries are used.
DEFINES += QT_DEPRECATED_WARNINGS

include(dependencies/dependencies.pri)
include(src/src.pri)

# For some reason it still compiles even if all these are explicitly removed.
# Included just for completion's sake, best to leave it be.
QT += core gui widgets

SOURCES += main.cpp
