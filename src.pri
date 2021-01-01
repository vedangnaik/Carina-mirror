lessThan(QT_MAJOR_VERSION, 5): error("Qt verion 5 or greater needed.")

SOURCES += \
    $$PWD/src/L2/usecases/SensorsManager.cpp \
    $$PWD/src/L2/usecases/ActuatorsManager.cpp \
    $$PWD/src/L2/usecases/StatesManager.cpp \
    $$PWD/src/L3/controllers/ActuatorsController.cpp \
    $$PWD/src/L3/controllers/StatesController.cpp \
    $$PWD/src/L3/gateways/ActuatorStatusGateway.cpp \
    $$PWD/src/L3/gateways/ProcessGateway.cpp \
    $$PWD/src/L3/gateways/SensorValuesGateway.cpp \
    $$PWD/src/L3/presenters/ActuatorsPresenter.cpp \
    $$PWD/src/L3/presenters/SensorsPresenter.cpp \
    $$PWD/src/L3/presenters/StatesPresenter.cpp \
    $$PWD/src/L4/dataacquisition/AiDAQHandler.cpp \
    $$PWD/src/L4/dataacquisition/DAQManager.cpp \
    $$PWD/src/L4/ui/ErrorUIHandler.cpp \
    $$PWD/src/L4/ui/GSManager.cpp \
    $$PWD/src/L4/ui/StateUIHandler.cpp \
    $$PWD/src/L4/ui/SystemDiagramUIhandler.cpp \
    $$PWD/src/L2/services/easylogging++.cc

HEADERS += \
    $$PWD/src/L2/usecases/SensorsManager.h \
    $$PWD/src/L2/usecases/ActuatorsManager.h \
    $$PWD/src/L2/usecases/StatesManager.h \
    $$PWD/src/L3/controllers/ActuatorsController.h \
    $$PWD/src/L3/controllers/StatesController.h \
    $$PWD/src/L3/gateways/ActuatorStatusGateway.h \
    $$PWD/src/L3/gateways/ProcessGateway.h \
    $$PWD/src/L3/gateways/SensorValuesGateway.h \
    $$PWD/src/L1/entities/Actuator.h \
    $$PWD/src/L1/entities/Sensor.h \
    $$PWD/src/L1/entities/State.h \
    $$PWD/src/L3/presenters/ActuatorsPresenter.h \
    $$PWD/src/L3/presenters/SensorsPresenter.h \
    $$PWD/src/L3/presenters/StatesPresenter.h \
    $$PWD/src/L4/dataacquisition/AiDAQHandler.h \
    $$PWD/src/L4/dataacquisition/DAQDeviceHandler.h \
    $$PWD/src/L4/dataacquisition/DAQManager.h \
    $$PWD/src/L4/ui/Draggable.h \
    $$PWD/src/L4/ui/ErrorUIHandler.h \
    $$PWD/src/L4/ui/GSManager.h \
    $$PWD/src/L4/ui/StateUIHandler.h \
    $$PWD/src/L4/ui/SystemDiagramUIhandler.h \
    $$PWD/src/L4/ui/UIHelpers.h \
    $$PWD/src/L2/services/easylogging++.h

FORMS += \
    $$PWD/src/L4/ui/error.ui \
    $$PWD/src/L4/ui/gsmainwindow.ui \
    $$PWD/src/L4/ui/state.ui \
    $$PWD/src/L4/ui/systemdiagram.ui

INCLUDEPATH += \
    $$PWD/src/L1/entities \
    $$PWD/src/L2/usecases \
    $$PWD/src/L2/services \
    $$PWD/src/L3/controllers \
    $$PWD/src/L3/presenters \
    $$PWD/src/L3/gateways \
    $$PWD/src/L4/dataacquisition \
    $$PWD/src/L4/ui

CONFIG += c++17

CONFIG += warn_on
QMAKE_CXXFLAGS += -pedantic

unix:!macx: LIBS += -luldaq

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
