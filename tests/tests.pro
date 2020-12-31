include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt

HEADERS += \
    test_Actuator.h \
    test_ActuatorsManager.h

SOURCES += \
    main.cpp
