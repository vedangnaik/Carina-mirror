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

    QMainWindow* GSMainWindow = new QMainWindow();


    SensorsManager* sm = new SensorsManager();
    ActuatorsManager* am = new ActuatorsManager();
    ProcessManager* pm = new ProcessManager();
    ProcessController* pc = new ProcessController(sm, am, pm);
    GSMainWindowHandler* gsmwh = new GSMainWindowHandler(GSMainWindow, pc);
    ProcessPresenter* pp = new ProcessPresenter(sm, am, gsmwh);
    pm->setOutputContract(pp);

    ClocksModule* cm = new ClocksModule();

    GSMainWindow->show();

    return a.exec();
}
