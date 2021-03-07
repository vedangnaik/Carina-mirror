SOURCES += \
    $$PWD/dataacquisition/AbstractDAQ.cpp \
    $$PWD/dataacquisition/AiMCCDAQ.cpp \
    $$PWD/dataacquisition/DAQManager.cpp \
    $$PWD/dataacquisition/DummyDAQ.cpp \
    $$PWD/dataacquisition/I2CDAQ.cpp \
    $$PWD/dataacquisition/SerialPortDAQ.cpp \
    $$PWD/ui/DAQManagerWizard/DAQCalibrationPage.cpp \
    $$PWD/ui/DAQManagerWizard/DAQLinkingPage.cpp \
    $$PWD/ui/DAQManagerWizard/DAQManagerWizard.cpp \
    $$PWD/ui/DAQManagerWizard/DAQScanPage.cpp \
    $$PWD/ui/ErrorUIHandler.cpp \
    $$PWD/ui/GSManager.cpp \
    $$PWD/ui/StateUIHandler.cpp \
    $$PWD/ui/SystemDiagramUIHandler.cpp

HEADERS += \
    $$PWD/dataacquisition/AbstractDAQ.h \
    $$PWD/dataacquisition/AiMCCDAQ.h \
    $$PWD/dataacquisition/DAQManager.h \
    $$PWD/dataacquisition/DummyDAQ.h \
    $$PWD/dataacquisition/I2CDAQ.h \
    $$PWD/dataacquisition/SerialPortDAQ.h \
    $$PWD/ui/DAQManagerWizard/DAQCalibrationPage.h \
    $$PWD/ui/DAQManagerWizard/DAQLinkingPage.h \
    $$PWD/ui/DAQManagerWizard/DAQManagerWizard.h \
    $$PWD/ui/DAQManagerWizard/DAQScanPage.h \
    $$PWD/ui/Draggable.h \
    $$PWD/ui/ErrorUIHandler.h \
    $$PWD/ui/GSManager.h \
    $$PWD/ui/StateUIHandler.h \
    $$PWD/ui/SystemDiagramUIHandler.h \
    $$PWD/ui/UIHelpers.h

INCLUDEPATH += \
    $$PWD/dataacquisition \
    $$PWD/ui

# Add .ui forms for UI classes.
FORMS += \
    $$PWD/ui/DAQManagerWizard/DAQScanPage.ui \
    $$PWD/ui/Error.ui \
    $$PWD/ui/GSMainWindow.ui \
    $$PWD/ui/State.ui \
    $$PWD/ui/SystemDiagram.ui

# Runtime loading of .ui files required the uitools module.
#QT += uitools

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

# Link WiringPi for I2C stuff if it's available and required.
contains(DEFINES, WIRINGPI_AVAILABLE) {
    LIBS += -lwiringPi
}
