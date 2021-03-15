SOURCES += \
    $$PWD/dataacquisition/AbstractDAQ.cpp \
    $$PWD/dataacquisition/AiMCCDAQ.cpp \
    $$PWD/dataacquisition/DAQManager.cpp \
    $$PWD/dataacquisition/DummyDAQ.cpp \
    $$PWD/dataacquisition/I2CDAQ.cpp \
    $$PWD/dataacquisition/SerialPortDAQ.cpp \
    $$PWD/ui/DAQManagerConfiguration/DAQCalibrationDialog.cpp \
    $$PWD/ui/DAQManagerConfiguration/DAQLinkDialog.cpp \
    $$PWD/ui/DAQManagerConfiguration/DAQScanDialog.cpp \
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
    $$PWD/ui/DAQManagerConfiguration/DAQCalibrationDialog.h \
    $$PWD/ui/DAQManagerConfiguration/DAQLinkDialog.h \
    $$PWD/ui/DAQManagerConfiguration/DAQScanDialog.h \
    $$PWD/ui/Draggable.h \
    $$PWD/ui/ErrorUIHandler.h \
    $$PWD/ui/GSManager.h \
    $$PWD/ui/StateUIHandler.h \
    $$PWD/ui/SystemDiagramUIHandler.h \
    $$PWD/ui/UIHelpers.h

INCLUDEPATH += \
    $$PWD/dataacquisition \
    $$PWD/ui \
    $$PWD/ui/DAQManagerConfiguration

# Add .ui forms for UI classes.
FORMS += \
    $$PWD/ui/DAQManagerConfiguration/DAQCalibrationDialog.ui \
    $$PWD/ui/DAQManagerConfiguration/DAQScanDialog.ui \
    $$PWD/ui/Error.ui \
    $$PWD/ui/GSMainWindow.ui \
    $$PWD/ui/State.ui \
    $$PWD/ui/SystemDiagram.ui

# Link uldaq.h for DAQ classes, if it's required.
contains(DEFINES, ULDAQ_AVAILABLE) {
    LIBS += -luldaq
}

# Link WiringPi for I2C stuff if it's available and required.
contains(DEFINES, WIRINGPI_AVAILABLE) {
    LIBS += -lwiringPi
}
