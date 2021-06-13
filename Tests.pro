include(dependencies/dependencies.pri)
include(common_settings.pri)
include(src/L1/L1.pri)
include(src/L2/L2.pri)
include(src/L3/L3.pri)

QT += core gui widgets

SOURCES += \
    tests_main.cpp \
    ../googletest/googletest/src/gtest-all.cc \
    ../googletest/googlemock/src/gmock-all.cc \


# The headers appear to require the name "tst_..." to show up in Qt Creator's testing pane.
# Best to just stick to that pattern for now.
HEADERS += \
    $$PWD/tests/tst_ActuatorsManager.h \
    $$PWD/tests/tst_Actuator.h \
    $$PWD/tests/tst_Sensor.h


INCLUDEPATH += \
    ../googletest/googletest \
    ../googletest/googletest/include \
    ../googletest/googlemock \
    ../googletest/googlemock/include
