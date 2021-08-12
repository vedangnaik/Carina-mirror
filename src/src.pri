HEADERS += \
    $$PWD/L1/entities/Actuator.h \
    $$PWD/L1/entities/Sensor.h \
    $$PWD/L1/entities/State.h
INCLUDEPATH += \
    $$PWD/L1/entities


SOURCES += \
    $$PWD/L2/usecases/SensorsManager.cpp \
    $$PWD/L2/usecases/ActuatorsManager.cpp \
    $$PWD/L2/usecases/StatesManager.cpp
HEADERS += \
    $$PWD/L2/usecases/SensorsManager.h \
    $$PWD/L2/usecases/ActuatorsManager.h
INCLUDEPATH += \
    $$PWD/L2/usecases


SOURCES += \
    $$PWD/L3/controllers/ActuatorsController.cpp \
    $$PWD/L3/controllers/StatesController.cpp \
    $$PWD/L3/gateways/ProcessFileParser.cpp \
    $$PWD/L3/gateways/SensorValuesGateway.cpp \
    $$PWD/L3/presenters/ActuatorsPresenter.cpp \
    $$PWD/L3/presenters/SensorsPresenter.cpp \
    $$PWD/L3/presenters/StatesPresenter.cpp
HEADERS += \
    $$PWD/L3/controllers/ActuatorsController.h \
    $$PWD/L3/controllers/StatesController.h \
    $$PWD/L3/gateways/ProcessFileParser.h \
    $$PWD/L3/gateways/SensorValuesGateway.h
INCLUDEPATH += \
    $$PWD/L3/controllers \
    $$PWD/L3/presenters \
    $$PWD/L3/gateways


SOURCES += \
    $$PWD/L4/dataacquisition/AbstractDAQ.cpp \
    $$PWD/L4/dataacquisition/AiMCCDAQ.cpp \
    $$PWD/L4/dataacquisition/DAQManager.cpp \
    $$PWD/L4/dataacquisition/DummyDAQ.cpp \
    $$PWD/L4/dataacquisition/I2CDAQ.cpp \
    $$PWD/L4/dataacquisition/OneWireDAQ.cpp \
    $$PWD/L4/dataacquisition/SerialPortDAQ.cpp \
    $$PWD/L4/ui/DAQManagerConfiguration/DAQCalibrationDialog.cpp \
    $$PWD/L4/ui/DAQManagerConfiguration/DAQLinkDialog.cpp \
    $$PWD/L4/ui/DAQManagerConfiguration/DAQScanDialog.cpp \
    $$PWD/L4/ui/ErrorUIHandler.cpp \
    $$PWD/L4/ui/GSManager.cpp \
    $$PWD/L4/ui/StateUIHandler.cpp \
    $$PWD/L4/ui/SystemDiagramUIHandler.cpp \
    $$PWD/L4/control/Adafruit_PWMServoDriver.cpp \
    $$PWD/L4/control/AbstractActuator.cpp \
    $$PWD/L4/control/PCA9685Actuator.cpp
HEADERS += \
    $$PWD/L4/dataacquisition/AbstractDAQ.h \
    $$PWD/L4/dataacquisition/AiMCCDAQ.h \
    $$PWD/L4/dataacquisition/DAQManager.h \
    $$PWD/L4/dataacquisition/DummyDAQ.h \
    $$PWD/L4/dataacquisition/I2CDAQ.h \
    $$PWD/L4/dataacquisition/OneWireDAQ.h \
    $$PWD/L4/dataacquisition/SerialPortDAQ.h \
    $$PWD/L4/ui/DAQManagerConfiguration/DAQCalibrationDialog.h \
    $$PWD/L4/ui/DAQManagerConfiguration/DAQLinkDialog.h \
    $$PWD/L4/ui/DAQManagerConfiguration/DAQScanDialog.h \
    $$PWD/L4/ui/Draggable.h \
    $$PWD/L4/ui/ErrorUIHandler.h \
    $$PWD/L4/ui/GSManager.h \
    $$PWD/L4/ui/StateUIHandler.h \
    $$PWD/L4/ui/SystemDiagramUIHandler.h \
    $$PWD/L4/ui/UIHelpers.h \
    $$PWD/L4/control/Adafruit_PWMServoDriver.h \
    $$PWD/L4/control/AbstractActuator.h \
    $$PWD/L4/control/PCA9685Actuator.h
INCLUDEPATH += \
    $$PWD/L4/dataacquisition \
    $$PWD/L4/ui \
    $$PWD/L4/ui/DAQManagerConfiguration

# Add .ui forms for UI classes.
FORMS += \
    $$PWD/L4/ui/DAQManagerConfiguration/DAQCalibrationDialog.ui \
    $$PWD/L4/ui/DAQManagerConfiguration/DAQLinkDialog.ui \
    $$PWD/L4/ui/DAQManagerConfiguration/DAQScanDialog.ui \
    $$PWD/L4/ui/Error.ui \
    $$PWD/L4/ui/GSMainWindow.ui \
    $$PWD/L4/ui/State.ui \
    $$PWD/L4/ui/SystemDiagram.ui

# Link uldaq.h for DAQ classes, if it's required.
contains(DEFINES, ULDAQ_AVAILABLE) {
    LIBS += -luldaq
}

# Link WiringPi for I2C stuff if it's available and required.
contains(DEFINES, WIRINGPI_AVAILABLE) {
    LIBS += -lwiringPi
}
