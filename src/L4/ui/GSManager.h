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
#include "L2/usecases/SensorsManager.h"
#include "L2/usecases/ActuatorsManager.h"
#include "L2/usecases/StatesManager.h"
#include "L2/services/easylogging++.h"
// L3
#include "L3/controllers/ActuatorsController.h"
#include "L3/gateways/ProcessGateway.h"
#include "L3/gateways/SensorValuesGateway.h"
#include "L3/presenters/SensorsPresenter.h"
#include "L3/presenters/ActuatorsPresenter.h"
#include "L3/presenters/StatesPresenter.h"
// L4
#include "L4/dataacquisition/DAQManager.h"
#include "L4/ui/StateUIHandler.h"
#include "L4/ui/SystemDiagramUIhandler.h"
#include "L4/ui/UIHelpers.h"
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
