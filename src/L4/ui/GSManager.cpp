#include "GSManager.h"


GSManager::GSManager() {
    this->GSMainWindowUI.setupUi(this);
    this->stateUI.setupUi(this);
    this->systemDiagramUI.setupUi(this);

    this->GSMainWindowUI.stateFrame->layout()->addWidget(this->stateUI.StateWidget);
    this->GSMainWindowUI.systemDiagramFrame->layout()->addWidget(this->systemDiagramUI.systemDiagramFrame);

    connect(this->GSMainWindowUI.openProcessFromFileAction, &QAction::triggered, this, [=]() {
        QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open Process File"), "/home/vedang/Desktop/");
        if (fileName != "") {
            this->createProcess(fileName.toStdString());
            this->GSMainWindowUI.openProcessFromFileAction->setEnabled(false);
            this->GSMainWindowUI.startProcessAction->setEnabled(true);
        }
    });

    connect(this->GSMainWindowUI.startProcessAction, &QAction::triggered, this, [=]() {
        this->startProcess();
        this->GSMainWindowUI.startProcessAction->setEnabled(false);
        this->GSMainWindowUI.closeProcessAction->setEnabled(true);
    });

    connect(this->GSMainWindowUI.closeProcessAction, &QAction::triggered, this, [=]() {
        this->closeProcess();
        this->GSMainWindowUI.closeProcessAction->setEnabled(false);
        this->GSMainWindowUI.openProcessFromFileAction->setEnabled(true);
    });

    connect(this->GSMainWindowUI.openSystemDiagramAction, &QAction::triggered, this, [=]() {
        QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open System Diagram Image"), "/home/vedang/Desktop/");
        if (fileName != "") {
            this->systemDiagramUI.systemDiagramFrame->setStyleSheet("#systemDiagramFrame{ border-image: url(" + fileName + ") 0 0 0 0 }");
        }
    });

    connect(this->GSMainWindowUI.clearSystemDiagramAction, &QAction::triggered, this, [=]() {
        this->systemDiagramUI.systemDiagramFrame->setStyleSheet("");
    });
}

void GSManager::createProcess(std::string filepath) {
    ProcessGateway pg(filepath);
    struct ProcessData pgdata = pg.parseProcessFile();

    this->cm = new ClocksModule();
    this->sm = new SensorsManager(pgdata.sensors);
    this->am = new ActuatorsManager(pgdata.actuators);
    // check for invalid start state here
    this->stm = new StatesManager(pgdata.states, *this->sm, *this->am);

    this->svg = new SensorValuesGateway(*this->sm);
    this->ac = new ActuatorsController(*this->am);
    this->stc = new StatesController(*this->stm);

    this->suih = new StateUIHandler(this->stateUI, *this->ac, *this->stc, *this->cm);
    this->sduih = new SystemDiagramUIHandler(this->systemDiagramUI, *this->ac, *this->cm);
    this->sp = new SensorsPresenter({this->suih, this->sduih});
    this->stp = new StatesPresenter(*this->suih);

    this->sm->setOutputContract(this->sp);
    this->stm->setOutputContract(this->stp);

    this->daqp = new DAQPlaceholder(this->cm, this->svg);
}

void GSManager::startProcess() {
    this->stm->startProcess();
    this->daqp->startAcquisition();
    this->cm->start();
}

void GSManager::closeProcess() {

}
