#include <QApplication>
#include <QMainWindow>
#include <string>

// ENTITIES
#include "entities.h"
// USE CASES
#include "usecases.h"
// GATEWAYS
//#include "gateways.h"
// CONTROLLERS
#include "controllers.h"
// PRESENTERS
#include "presenters.h"
// UI
#include "userinterface.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    ProcessManager* pm = new ProcessManager();
    ProcessController* pc = new ProcessController();
    ProcessPresenter* pp = new ProcessPresenter();
    UserInterface* ui = new UserInterface(pc);

    pm->pmoc = pp;
    pp->ppoc = ui;
    pc->pmic = pm;
//    ui->pcic = pc;

    ui->show();

    return a.exec();
}
