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
// L2
#include "SensorsManager.h"
#include "ActuatorsManager.h"
#include "StatesManager.h"
// L3
#include "ActuatorsController.h"
#include "ProcessFileParser.h"
#include "SensorValuesGateway.h"
#include "SensorsPresenter.h"
#include "ActuatorsPresenter.h"
#include "StatesPresenter.h"
// L4
#include "DAQCalibrationDialog.h"
#include "DAQScanDialog.h"
#include "DAQLinkDialog.h"
#include "DAQManager.h"
#include "StateUIHandler.h"
#include "SystemDiagramUIHandler.h"
#include "ErrorUIHandler.h"
#include "UIHelpers.h"
#include "ui_GSMainWindow.h"
#include "ui_State.h"
#include "ui_SystemDiagram.h"
#include "ui_Error.h"

using std::map;
using std::string;
using std::make_unique;
using std::unique_ptr;

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
    unique_ptr<SensorsManager> sm;
    unique_ptr<ActuatorsManager> am;
    unique_ptr<StatesManager> stm;
    // L3
    unique_ptr<SensorValuesGateway> svg;
    unique_ptr<ActuatorsController> ac;
    unique_ptr<StatesController> stc;
    unique_ptr<SensorsPresenter> sp;
    unique_ptr<ActuatorsPresenter> ap;
    unique_ptr<StatesPresenter> stp;
    // L4
    unique_ptr<ErrorUIHandler> euih;
    unique_ptr<StateUIHandler> suih;
    unique_ptr<SystemDiagramUIHandler> sduih;
    unique_ptr<DAQManager> daqm;
};
