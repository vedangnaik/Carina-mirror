#pragma once

#include <QObject>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QHBoxLayout>
#include <iostream>
// dependencies
#include <easylogging++.h>
// L1
#include "Sensor.h"
#include "Actuator.h"
// L2
#include "SensorsManager.h"
#include "ActuatorsManager.h"
#include "StatesManager.h"
// L3
#include "ActuatorsController.h"
#include "ProcessFileParser.h"
#include "SensorsPresenter.h"
#include "ActuatorsPresenter.h"
#include "StatesPresenter.h"
// L4
#include "ConcreteSensorFactory.h"
#include "ConcreteActuatorFactory.h"
#include "StateUIHandler.h"
#include "SystemDiagramUIHandler.h"
#include "ErrorUIHandler.h"
#include "UIHelpers.h"
#include "ui_GSMainWindow.h"
#include "ui_State.h"
#include "ui_SystemDiagram.h"
#include "ui_Error.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class GSMainWindow;
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
    std::unique_ptr<ActuatorsController> ac;
    std::unique_ptr<StatesController> stc;
    std::unique_ptr<SensorsPresenter> sp;
    std::unique_ptr<ActuatorsPresenter> ap;
    std::unique_ptr<StatesPresenter> stp;
    // L4
    std::unique_ptr<ErrorUIHandler> euih;
    std::unique_ptr<StateUIHandler> suih;
    std::unique_ptr<SystemDiagramUIHandler> sduih;
};
