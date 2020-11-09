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
    src/adapters/controllers/actuatorsController.cpp \
    src/adapters/controllers/processController.cpp \
    src/adapters/gateways/processGateway.cpp \
    src/adapters/presenters/processPresenter.cpp \
    src/entities/entities.cpp \
    src/external/services/clocksModule.cpp \
    src/external/ui/GSMainWindowHandler.cpp \
    src/external/ui/ProcessUIHandler.cpp \
    src/external/ui/StateUIHandler.cpp \
    src/main.cpp \
    src/usecases/actuatorsManager.cpp \
    src/usecases/processManager.cpp \
    src/usecases/sensorsManager.cpp

HEADERS += \
    src/entities/entities.h \
    src/external/services/services.h \
    src/usecases/usecases.h \
    src/adapters/controllers/controllers.h \
    src/adapters/presenters/presenters.h \
    src/adapters/gateways/gateways.h \
    src/external/ui/userinterface.h

FORMS += \
    src/external/ui/gsmainwindow.ui \
    src/external/ui/processsummary.ui \
    src/external/ui/state.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
