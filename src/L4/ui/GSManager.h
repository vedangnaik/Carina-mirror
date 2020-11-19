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
#include "src/L2/usecases/SensorsManager.h"
#include "src/L2/usecases/ActuatorsManager.h"
#include "src/L2/usecases/StatesManager.h"
#include "src/L2/services/ClocksModule.h"
// L3
#include "src/L3/controllers/ActuatorsController.h"
#include "src/L3/gateways/ProcessGateway.h"
#include "src/L3/gateways/SensorValuesGateway.h"
#include "src/L3/presenters/StatesPresenter.h"
#include "src/L3/presenters/SensorsPresenter.h"
// L4
#include "src/L4/daq/DAQPlaceHolder.h"
#include "src/L4/ui/StateUIHandler.h"


QT_BEGIN_NAMESPACE
namespace Ui {
    class GSMainWindow;
    class State;
    // class SystemDiagram
}
QT_END_NAMESPACE

class GSManager : public QMainWindow {
    Q_OBJECT
public:
    GSManager();
    ~GSManager();

    void createProcess(std::string filepath);
    void startProcess();
    void closeProcess();
private:
    Ui::GSMainWindow* mainWindowUI;
    Ui::State* stateUI;
    // L2
    ClocksModule* cm;
    SensorsManager* sm;
    ActuatorsManager* am;
    StatesManager* stm;
    // L3
    SensorValuesGateway* svg;
    ActuatorsController* ac;
    StatesController* stc;
    SensorsPresenter* sp;
    StatesPresenter* stp;
    // L4
    StateUIHandler* suih;
    DAQPlaceholder* daqp;
};

#endif // GSMAINWINDOWHANDLER_H
