SOURCES += \
    $$PWD/dataacquisition/AiDAQHandler.cpp \
    $$PWD/dataacquisition/SerialPortHandler.cpp \
    $$PWD/dataacquisition/DAQManager.cpp \
    $$PWD/ui/DAQManagerFactory.cpp \
    $$PWD/ui/ErrorUIHandler.cpp \
    $$PWD/ui/GSManager.cpp \
    $$PWD/ui/SensorToDAQLinker.cpp \
    $$PWD/ui/StateUIHandler.cpp \
    $$PWD/ui/SystemDiagramUIhandler.cpp

HEADERS += \
    $$PWD/dataacquisition/AiDAQHandler.h \
    $$PWD/dataacquisition/IDAQDeviceHandler.h \
    $$PWD/dataacquisition/SerialPortHandler.h \
    $$PWD/dataacquisition/DAQManager.h \
    $$PWD/ui/DAQManagerFactory.h \
    $$PWD/ui/Draggable.h \
    $$PWD/ui/ErrorUIHandler.h \
    $$PWD/ui/GSManager.h \
    $$PWD/ui/SensorToDAQLinker.h \
    $$PWD/ui/StateUIHandler.h \
    $$PWD/ui/SystemDiagramUIhandler.h \
    $$PWD/ui/UIHelpers.h

INCLUDEPATH += \
    $$PWD/dataacquisition \
    $$PWD/ui

# Add .ui forms for UI classes.
FORMS += \
    $$PWD/ui/SensorToDAQLinker.ui \
    $$PWD/ui/daqmanagerfactory.ui \
    $$PWD/ui/error.ui \
    $$PWD/ui/gsmainwindow.ui \
    $$PWD/ui/state.ui \
    $$PWD/ui/systemdiagram.ui

# Runtime loading of .ui files required the uitools module.
QT += uitools

# Defines the variable which includes uldaq.h and the associated code for reading real DAQs.
# Turn it on if you want to test real DAQs; it's off by default since uldaq.h is not included
# by default on any Linux distros and cannot be used on Windows at all.
# Note: This doesn't have to be done here, it can also be passed in as an argument to qmake.
# It's just put here because it's the most relevant spot.
#DEFINES += ULDAQ_AVAILABLE

# Link uldaq.h for DAQ classes, if it's required.
contains(DEFINES, ULDAQ_AVAILABLE) {
    LIBS += -luldaq
}
