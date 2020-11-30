#include "GSManager.h"

GSManager::GSManager() {
    this->renderUi();

    connect(this->GSMainWindowUI.openProcessFromFileAction, &QAction::triggered, this, [=]() {
        QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open Process File"), "/home/vedang/Desktop/");
        if (fileName != "") {
            this->openProcessFromFile(fileName.toStdString());
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

void GSManager::openProcessFromFile(std::string filepath) {
    // Only use for Process Gateway
    ProcessGateway pg(filepath);
    struct ProcessData pgdata = pg.parseProcessFile();
    // init L2 classes here
    this->cm = new ClocksModule();
    this->sm = new SensorsManager(pgdata.sensors);
    this->am = new ActuatorsManager(pgdata.actuators);
    // check for invalid start state here
    this->stm = new StatesManager(pgdata.states, *this->sm, *this->am);
    // init L3 classes here
    this->svg = new SensorValuesGateway(*this->sm);
    this->ac = new ActuatorsController(*this->am);
    this->stc = new StatesController(*this->stm);
    // init L4 and presenters here
    this->sp = new SensorsPresenter(*this->cm);
    this->suih = new StateUIHandler(this->stateUI, *this->sp, *this->ac, *this->stc, *this->cm);
    // SystemDiagramUI
    this->stp = new StatesPresenter(*this->suih);
    this->daqp = new DAQPlaceholder(this->cm, this->svg);
    // attach presenters to managers (kinda ugly, but idk another way to do it)
    this->sm->setOutputContract(this->sp);
    this->stm->setOutputContract(this->stp);
}

void GSManager::startProcess() {
    this->stm->startProcess();
    this->daqp->startAcquisition();
    this->cm->start();
}

void GSManager::stopProcess() {
    this->cm->stop();
    this->daqp->stopAcquisition();
    this->stm->stopProcess();
}

void GSManager::closeProcess() {
    this->stopProcess();
    // these deletes are in the opposite order
    // to the constructs in openProcess().
    delete this->daqp;
    delete this->stp;
    delete this->sp;
    delete this->sduih;
    delete this->suih;
    delete this->stc;
    delete this->ac;
    delete this->svg;
    delete this->stm;
    delete this->am;
    delete this->sm;
    delete this->cm;
    // rerender the UI for the next process.
    this->rerenderUi();
}

void GSManager::renderUi() {
    this->GSMainWindowUI.setupUi(this);
    this->stateUI.setupUi(this->GSMainWindowUI.stateFrame);
    this->systemDiagramUI.setupUi(this->GSMainWindowUI.systemDiagramFrame);
}

// this function surgically removes only the necessary bits instead of just
// overwriting everything using renderUi to preserve the signal slot connects
// made in the constructor.
void GSManager::rerenderUi() {
    // clear stateFrame's layout, delete it, then remake and re-add the state UI.
    uihelpers::clearLayout(this->GSMainWindowUI.stateFrame->layout());
    delete this->GSMainWindowUI.stateFrame->layout();
    this->stateUI.setupUi(this->GSMainWindowUI.stateFrame);
    // same as above for systemDiagramFrame.
    uihelpers::clearLayout(this->GSMainWindowUI.systemDiagramFrame->layout());
    delete this->GSMainWindowUI.systemDiagramFrame->layout();
    this->systemDiagramUI.setupUi(this->GSMainWindowUI.systemDiagramFrame);
}
