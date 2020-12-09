QT += core gui
QT += uitools

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/L2/usecases/StatesManager.cpp \
    src/L3/controllers/ActuatorsController.cpp \
    src/L3/controllers/StatesController.cpp \
    src/L3/gateways/ActuatorStatusGateway.cpp \
    src/L3/gateways/ProcessGateway.cpp \
    src/L3/gateways/SensorValuesGateway.cpp \
    src/L3/presenters/ActuatorsPresenter.cpp \
    src/L3/presenters/SensorsPresenter.cpp \
    src/L3/presenters/StatesPresenter.cpp \
    src/L4/dataacquisition/DAQPlaceHolder.cpp \
    src/L4/dataacquisition/DAQHandler.cpp \
    src/L4/ui/GSManager.cpp \
    src/L4/ui/StateUIHandler.cpp \
    src/L4/ui/SystemDiagramUIhandler.cpp \
    src/main.cpp \
    src/L2/usecases/ActuatorsManager.cpp \
    src/L2/usecases/SensorsManager.cpp \
    src/L2/services/ClocksModule.cpp

HEADERS += \
    src/L2/usecases/StatesManager.h \
    src/L3/controllers/ActuatorsController.h \
    src/L3/controllers/StatesController.h \
    src/L3/gateways/ActuatorStatusGateway.h \
    src/L3/gateways/ProcessGateway.h \
    src/L3/gateways/SensorValuesGateway.h \
    src/L1/entities/Actuator.h \
    src/L1/entities/Sensor.h \
    src/L1/entities/State.h \
    src/L3/presenters/ActuatorsPresenter.h \
    src/L3/presenters/SensorsPresenter.h \
    src/L3/presenters/StatesPresenter.h \
    src/L4/dataacquisition/DAQPlaceHolder.h \
    src/L4/dataacquisition/DAQHandler.h \
    src/L4/ui/Draggable.h \
    src/L4/ui/GSManager.h \
    src/L4/ui/StateUIHandler.h \
    src/L2/usecases/ActuatorsManager.h \
    src/L2/usecases/SensorsManager.h \
    src/L2/services/ClocksModule.h \
    src/L4/ui/SystemDiagramUIhandler.h \
    src/L4/ui/UIHelpers.h

FORMS += \
    src/L4/ui/gsmainwindow.ui \
    src/L4/ui/state.ui \
    src/L4/ui/systemdiagram.ui

unix:!macx: LIBS += -luldaq

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
