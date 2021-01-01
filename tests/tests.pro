include(gtest_dependency.pri)
include(../src.pri)

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG += thread
#CONFIG -= qt

QT += core gui uitools widgets

SOURCES += $$PWD/main.cpp

HEADERS += $$PWD/tst_basicproperties.h
