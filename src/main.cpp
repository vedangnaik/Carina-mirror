#include <QApplication>
#include <QMainWindow>
#include <string>

// ENTITIES
#include "src/entities/entities.h"
// USE CASES
#include "src/usecases/usecases.h"
// ADAPTERS
#include "src/adapters/gateways/gateways.h"
#include "src/adapters/controllers/controllers.h"
#include "src/adapters/presenters/presenters.h"
// EXTERNAL
#include "src/external/ui/userinterface.h"
#include "src/external/services/services.h"


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
