#include <QApplication>
#include <QMainWindow>
#include <string>
// L1
// L2
#include "src/L2/usecases/SensorsManager.h"
#include "src/L2/usecases/ActuatorsManager.h"
#include "src/L2/usecases/ProcessManager.h"
#include "src/L2/services/ClocksModule.h"
// L3
#include "src/L3/controllers/ActuatorsController.h"
#include "src/L3/controllers/ProcessController.h"
#include "src/L3/gateways/ProcessGateway.h"
#include "src/L3/gateways/SensorValuesGateway.h"
#include "src/L3/presenters/ProcessPresenter.h"
// L4
#include "src/L4/daq/DAQPlaceHolder.h"
#include "src/L4/ui/GSMainWindowHandler.h"
#include "src/L4/ui/ProcessUIHandler.h"
#include "src/L4/ui/StateUIHandler.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    ClocksModule* cm = new ClocksModule();
    SensorsManager* sm = new SensorsManager();
    ActuatorsManager* am = new ActuatorsManager();
    ProcessManager* pm = new ProcessManager(am, sm, cm);

    ProcessController* pc = new ProcessController(pm);
    ActuatorsController* ac = new ActuatorsController(am);
    SensorValuesGateway* svg = new SensorValuesGateway(sm);

    DAQPlaceholder* daqp = new DAQPlaceholder(cm, svg);
    QMainWindow* GSMainWindow = new QMainWindow();
    GSMainWindowHandler* gsmwh = new GSMainWindowHandler(GSMainWindow, pc);
    StateUIHandler* suih = new StateUIHandler(gsmwh, ac, cm);
    ProcessUIHandler* puih = new ProcessUIHandler(gsmwh, suih, pc);

    ProcessPresenter* pp = new ProcessPresenter(puih);

    pm->setOutputContract(pp);

    daqp->startAcquisition();
    GSMainWindow->show();
    return a.exec();
}
