SOURCES += \
    $$PWD/dataacquisition/AiDAQHandler.cpp \
    $$PWD/dataacquisition/DAQManager.cpp \
    $$PWD/ui/ErrorUIHandler.cpp \
    $$PWD/ui/GSManager.cpp \
    $$PWD/ui/StateUIHandler.cpp \
    $$PWD/ui/SystemDiagramUIhandler.cpp

HEADERS += \
    $$PWD/dataacquisition/AiDAQHandler.h \
    $$PWD/dataacquisition/DAQDeviceHandler.h \
    $$PWD/dataacquisition/DAQManager.h \
    $$PWD/ui/Draggable.h \
    $$PWD/ui/ErrorUIHandler.h \
    $$PWD/ui/GSManager.h \
    $$PWD/ui/StateUIHandler.h \
    $$PWD/ui/SystemDiagramUIhandler.h \
    $$PWD/ui/UIHelpers.h

INCLUDEPATH += \
    $$PWD/dataacquisition \
    $$PWD/ui

# Add .ui forms for UI classes.
FORMS += \
    $$PWD/ui/error.ui \
    $$PWD/ui/gsmainwindow.ui \
    $$PWD/ui/state.ui \
    $$PWD/ui/systemdiagram.ui

# Runtime loading of .ui files required the uitools module.
QT += uitools

# Link uldaq.h for DAQ classes.
unix:!macx: LIBS += -luldaq
