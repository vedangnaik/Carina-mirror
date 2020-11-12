QT += core gui
QT += uitools

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

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
    src/adapters/controllers/ActuatorsController.cpp \
    src/adapters/controllers/ProcessController.cpp \
    src/adapters/gateways/ProcessGateway.cpp \
    src/adapters/presenters/ProcessPresenter.cpp \
    src/entities/Actuator.cpp \
    src/entities/Process.cpp \
    src/entities/Sensor.cpp \
    src/external/services/ClocksModule.cpp \
    src/external/ui/GSMainWindowHandler.cpp \
    src/external/ui/ProcessUIHandler.cpp \
    src/external/ui/StateUIHandler.cpp \
    src/main.cpp \
    src/usecases/ActuatorsManager.cpp \
    src/usecases/ProcessManager.cpp \
    src/usecases/SensorsManager.cpp

HEADERS += \
    src/adapters/controllers/ActuatorController.h \
    src/adapters/controllers/ProcessController.h \
    src/adapters/gateways/ProcessGateway.h \
    src/adapters/presenters/ProcessPresenter.h \
    src/entities/Actuator.h \
    src/entities/Process.h \
    src/entities/Sensor.h \
    src/entities/State.h \
    src/external/services/ClocksModule.h \
    src/external/ui/GSMainWindowHandler.h \
    src/external/ui/ProcessUIHandler.h \
    src/external/ui/StateUIHandler.h \
    src/usecases/ActuatorsManager.h \
    src/usecases/ProcessManager.h \
    src/usecases/SensorsManager.h

FORMS += \
    src/external/ui/gsmainwindow.ui \
    src/external/ui/processsummary.ui \
    src/external/ui/state.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
