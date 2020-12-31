#ifndef GSMAINWINDOWHANDLER_H
#define GSMAINWINDOWHANDLER_H

#include <QObject>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QtUiTools>
#include <QHBoxLayout>
#include <iostream>
// L1
// L2
#include "SensorsManager.h"
#include "ActuatorsManager.h"
#include "StatesManager.h"
#include "easylogging++.h"
// L3
#include "ActuatorsController.h"
#include "ProcessGateway.h"
#include "SensorValuesGateway.h"
#include "SensorsPresenter.h"
#include "ActuatorsPresenter.h"
#include "StatesPresenter.h"
// L4
#include "DAQManager.h"
#include "StateUIHandler.h"
#include "SystemDiagramUIhandler.h"
#include "UIHelpers.h"
#include "ui_gsmainwindow.h"
#include "ui_state.h"
#include "ui_systemdiagram.h"
#include "ui_error.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class GSMainWindow;
    class State;
    class SystemDiagram;
}
QT_END_NAMESPACE

class GSManager : public QMainWindow {
    Q_OBJECT
public:
    GSManager();
private:
    void openProcessFromFile(std::string filepath);
    void startProcess();
    void stopAndCloseProcess();
    void renderUi();
    void rerenderUi();
    // Ui
    Ui::GSMainWindow GSMainWindowUI;
    Ui::State stateUI;
    Ui::SystemDiagram systemDiagramUI;
    Ui::Error errorUI;
    // L2
    std::unique_ptr<SensorsManager> sm;
    std::unique_ptr<ActuatorsManager> am;
    std::unique_ptr<StatesManager> stm;
    // L3
    std::unique_ptr<SensorValuesGateway> svg;
    std::unique_ptr<ActuatorsController> ac;
    std::unique_ptr<StatesController> stc;
    std::unique_ptr<SensorsPresenter> sp;
    std::unique_ptr<ActuatorsPresenter> ap;
    std::unique_ptr<StatesPresenter> stp;
    // L4
    std::unique_ptr<StateUIHandler> suih;
    std::unique_ptr<SystemDiagramUIHandler> sduih;
    std::unique_ptr<DAQManager> daqm;
};

#endif // GSMAINWINDOWHANDLER_H
