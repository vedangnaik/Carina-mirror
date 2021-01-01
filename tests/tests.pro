include(gtest_dependency.pri)
include(../common_settings.pri)
include(../src/L1/L1.pri)
include(../src/L2/L2.pri)

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt

QT += core gui uitools widgets

SOURCES += $$PWD/main.cpp

HEADERS += $$PWD/tst_basicproperties.h
