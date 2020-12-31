QT += core gui
QT += uitools

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#INCLUDEPATH += \
#    ../../googletest/googletest \
#    ../../googletest/googlemock \
#    ../../googletest/googletest/include \
#    ../../googletest/googlemock/include \


CONFIG += c++17

QMAKE_CXXFLAGS += -pedantic

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
    L2/usecases/StatesManager.cpp \
    L3/controllers/ActuatorsController.cpp \
    L3/controllers/StatesController.cpp \
    L3/gateways/ActuatorStatusGateway.cpp \
    L3/gateways/ProcessGateway.cpp \
    L3/gateways/SensorValuesGateway.cpp \
    L3/presenters/ActuatorsPresenter.cpp \
    L3/presenters/SensorsPresenter.cpp \
    L3/presenters/StatesPresenter.cpp \
    L4/dataacquisition/AiDAQHandler.cpp \
    L4/dataacquisition/DAQManager.cpp \
    L4/ui/ErrorUIHandler.cpp \
    L4/ui/GSManager.cpp \
    L4/ui/StateUIHandler.cpp \
    L4/ui/SystemDiagramUIhandler.cpp \
    main.cpp \
    L2/usecases/ActuatorsManager.cpp \
    L2/usecases/SensorsManager.cpp \
    L2/services/easylogging++.cc
#    ../../googletest/googletest/src/gtest-all.cc \
#    ../../googletest/googlemock/src/gmock-all.cc

HEADERS += \
    L2/usecases/StatesManager.h \
    L3/controllers/ActuatorsController.h \
    L3/controllers/StatesController.h \
    L3/gateways/ActuatorStatusGateway.h \
    L3/gateways/ProcessGateway.h \
    L3/gateways/SensorValuesGateway.h \
    L1/entities/Actuator.h \
    L1/entities/Sensor.h \
    L1/entities/State.h \
    L3/presenters/ActuatorsPresenter.h \
    L3/presenters/SensorsPresenter.h \
    L3/presenters/StatesPresenter.h \
    L4/dataacquisition/AiDAQHandler.h \
    L4/dataacquisition/DAQDeviceHandler.h \
    L4/dataacquisition/DAQManager.h \
    L4/ui/Draggable.h \
    L4/ui/ErrorUIHandler.h \
    L4/ui/GSManager.h \
    L4/ui/StateUIHandler.h \
    L2/usecases/ActuatorsManager.h \
    L2/usecases/SensorsManager.h \
    L4/ui/SystemDiagramUIhandler.h \
    L4/ui/UIHelpers.h \
    L2/services/easylogging++.h

FORMS += \
    L4/ui/error.ui \
    L4/ui/gsmainwindow.ui \
    L4/ui/state.ui \
    L4/ui/systemdiagram.ui

unix:!macx: LIBS += -luldaq

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
