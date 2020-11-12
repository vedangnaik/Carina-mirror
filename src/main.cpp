#include <QApplication>
#include <QMainWindow>
#include <string>

// ENTITIES

// USE CASES
#include "src/usecases/SensorsManager.h"
#include "src/usecases/ActuatorsManager.h"
#include "src/usecases/ProcessManager.h"
// ADAPTERS
#include "src/adapters/controllers/ActuatorController.h"
#include "src/adapters/controllers/ProcessController.h"
#include "src/adapters/gateways/ProcessGateway.h"
#include "src/adapters/presenters/ProcessPresenter.h"
// EXTERNAL
#include "src/external/ui/GSMainWindowHandler.h"
#include "src/external/ui/ProcessUIHandler.h"
#include "src/external/ui/StateUIHandler.h"
#include "src/external/services/ClocksModule.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    ClocksModule* cm = new ClocksModule();

    SensorsManager* sm = new SensorsManager();
    ActuatorsManager* am = new ActuatorsManager();
    ProcessManager* pm = new ProcessManager(am, sm);

    ProcessController* pc = new ProcessController(sm, am, pm);
    ActuatorsController* ac = new ActuatorsController(am);

    QMainWindow* GSMainWindow = new QMainWindow();
    GSMainWindowHandler* gsmwh = new GSMainWindowHandler(GSMainWindow, pc);
    StateUIHandler* suih = new StateUIHandler(gsmwh, ac, cm);
    ProcessUIHandler* puih = new ProcessUIHandler(gsmwh, suih, pc);

    ProcessPresenter* pp = new ProcessPresenter(sm, am, puih);

    pm->setOutputContract(pp);


    GSMainWindow->show();


    return a.exec();
}
