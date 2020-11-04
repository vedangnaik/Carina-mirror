//#include "gsui.h"

#include <QApplication>
#include <QMainWindow>
#include <string>

// ENTITIES
#include "entities.h"
// USE CASES
#include "usecases.h"
// CONTROLLERS
#include "layer3.h"
// VIEWS
#include "presenters.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    ProcessManager* pm;
    SensorsManager* sm;

    ProcessController* pc;
    ProcessPresenter* pp;

    pp = new ProcessPresenter();
    pm = new ProcessManager(pp);
    sm = new SensorsManager();
    pc = new ProcessController(pm);
    pp->pm = pm;
    pp->sm = sm;

    pp->show();
    return a.exec();
}
