include(gtest_dependency.pri)
#include(../src/src_common.pri)

TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt

HEADERS += \
    test_Actuator.h \
    test_ActuatorsManager.h

SOURCES += \
    main.cpp

SOURCES += \
    ../src/L2/usecases/SensorsManager.cpp \
    ../src/L2/usecases/ActuatorsManager.cpp \
    ../src/L2/usecases/StatesManager.cpp \
    ../src/L2/services/easylogging++.cc \
    ../src/L4/ui/ErrorUIHandler.cpp

HEADERS += \
    ../src/L2/usecases/SensorsManager.h \
    ../src/L2/usecases/ActuatorsManager.h \
    ../src/L2/usecases/StatesManager.h \
    ../src/L2/services/easylogging++.h \
    ../src/L4/ui/ErrorUIHandler.h

INCLUDEPATH += \
    ../src/L1/entities \
    ../src/L2/usecases \
    ../src/L2/services \
    ../src/L4/ui
