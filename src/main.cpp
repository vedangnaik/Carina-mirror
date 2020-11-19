#include <QApplication>
#include "src/L4/ui/GSManager.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    GSManager gsm;
    gsm.show();

//    ProcessManager pm;
//    GSMainWindowHandler gsmwh(pm);


//    ProcessManager pm;
//    ProcessController pc(pm);
//    GSMainWindowHandler* gsmwh = new GSMainWindowHandler(pc);

//    ClocksModule* cm = new ClocksModule();
//    SensorsManager* sm = new SensorsManager();
//    ActuatorsManager* am = new ActuatorsManager();
//    StatesManager* pm = new StatesManager(am, sm, cm);


//    ActuatorsController* ac = new ActuatorsController(am);
//    SensorValuesGateway* svg = new SensorValuesGateway(sm);

//    DAQPlaceholder* daqp = new DAQPlaceholder(cm, svg);

//    StateUIHandler* suih = new StateUIHandler(gsmwh, ac, sm, cm);
//    ProcessUIHandler* puih = new ProcessUIHandler(gsmwh, suih, pc);

//    ProcessPresenter* pp = new ProcessPresenter(puih);

//    pm->setOutputContract(pp);

//    daqp->startAcquisition();
//    gsmwh->show();
    return a.exec();
}
