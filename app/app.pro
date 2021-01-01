include(../src/L1/L1.pri)
include(../src/L2/L2.pri)
include(../src/L3/L3.pri)
include(../src/L4/L4.pri)
include(../common_settings.pri)

QT += core gui uitools widgets
SOURCES += $$PWD/main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
