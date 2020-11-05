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

    ProcessManager* pm = new ProcessManager();
    ProcessController* pc = new ProcessController();
    ProcessPresenter* pp = new ProcessPresenter();
    UserInterface* ui = new UserInterface(pc);

    pm->pmoc = pp;
    pp->ppoc = ui;
    pc->pmic = pm;

    ui->show();

    return a.exec();
}
