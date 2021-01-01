SOURCES += \
    $$PWD/usecases/SensorsManager.cpp \
    $$PWD/usecases/ActuatorsManager.cpp \
    $$PWD/usecases/StatesManager.cpp \
    $$PWD/services/easylogging++.cc

HEADERS += \
    $$PWD/usecases/SensorsManager.h \
    $$PWD/usecases/ActuatorsManager.h \
    $$PWD/services/easylogging++.h

INCLUDEPATH += \
    $$PWD/usecases \
    $$PWD/services

#DEFINES += ELPP_CUSTOM_COUT=euih
//#define ELPP_CUSTOM_COUT_LINE(logLine) logLine
