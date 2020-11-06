#include <QApplication>
#include <QMainWindow>
#include <string>

// ENTITIES
#include "src/entities/entities.h"
// USE CASES
#include "src/usecases/usecases.h"
// GATEWAYS
//#include "gateways.h"
// CONTROLLERS
#include "src/adapters/controllers/controllers.h"
// PRESENTERS
#include "src/adapters/presenters/presenters.h"
// UI
#include "src/external/ui/userinterface.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    SensorsManager* sm = new SensorsManager();
    ActuatorsManager* am = new ActuatorsManager();
    ProcessManager* pm = new ProcessManager();
    ProcessController* pc = new ProcessController(sm, am, pm);
    UserInterface* ui = new UserInterface(pc);
    ProcessPresenter* pp = new ProcessPresenter(sm, am, ui);
    pm->setOutputContract(pp);

    ui->show();

    return a.exec();
}
