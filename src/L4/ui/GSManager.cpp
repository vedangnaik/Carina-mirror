#include "GSManager.h"
#include "ui_gsmainwindow.h"
#include "ui_state.h"


GSManager::GSManager() {
    this->mainWindowUI = new Ui::GSMainWindow;
    mainWindowUI->setupUi(this);

    connect(mainWindowUI->openProcessFromFileAction, &QAction::triggered, this, [&]() {
        QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open Process File"), "/home/vedang/Desktop/");
        if (fileName != "") {
            this->createProcess(fileName.toStdString());
        }
    });
}

GSManager::~GSManager() {
    delete mainWindowUI;
}

//QHBoxLayout* GSMainWindowHandler::getAbortButtonLayout() {
//    return this->mainWindowUI->abortButtonLayout;
//}

//QHBoxLayout* GSMainWindowHandler::getProcessSummaryLayout() {
//    return this->mainWindowUI->processSummaryLayout;
//}

//QHBoxLayout* GSMainWindowHandler::getProceedButtonLayout() {
//    return this->mainWindowUI->proceedButtonLayout;
//}

//QVBoxLayout* GSMainWindowHandler::getCurrentStateLayout() {
//    return this->mainWindowUI->currentStateLayout;
//}

void GSManager::createProcess(std::string filepath) {
    ProcessGateway pg(filepath);
    struct ProcessData pgdata = pg.parseProcessFile();

    this->cm = new ClocksModule();
    this->sm = new SensorsManager(pgdata.sensors);
    this->am = new ActuatorsManager(pgdata.actuators);
    // check for invalid start state here
    this->stm = new StatesManager(pgdata.states, *this->sm, *this->am, *this->cm);

    this->svg = new SensorValuesGateway(this->sm);
    this->ac = new ActuatorsController(this->am);
    // controller for states here

    this->suih = new StateUIHandler(new Ui::State, this->ac, this->sm, this->cm);


}

void GSManager::closeProcess() {

}

void GSManager::startProcess() {

}
