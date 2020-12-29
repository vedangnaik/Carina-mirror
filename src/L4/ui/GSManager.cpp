#include "GSManager.h"

GSManager::GSManager() {
    this->renderUi();

    connect(this->GSMainWindowUI.openProcessFromFileAction, &QAction::triggered, this, [=]() {
        QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open Process File"), "/", tr("JSON Files (*.json)"));
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
            tr("Open System Diagram Image"), "/", tr("Image Files (*.png *.jpg)"));
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

    // create array of sensor and actuator ids here
    std::vector<std::string> sensorIds, actuatorIds;
    for (const auto& [id, _] : pgdata.sensors) { sensorIds.push_back(id); }
    for (const auto& [id, _] : pgdata.actuators) { actuatorIds.push_back(id); }

    // init L2 classes here
    this->sm = new SensorsManager(pgdata.sensors);
    this->am = new ActuatorsManager(pgdata.actuators);
    // check for invalid start state here
    this->stm = new StatesManager(pgdata.states, *this->sm, *this->am);
    // init L3 classes here
    this->svg = new SensorValuesGateway(*this->sm);
    this->ac = new ActuatorsController(*this->am);
    this->stc = new StatesController(*this->stm);
    // init L4 and presenters here
    this->sp = new SensorsPresenter();
    this->ap = new ActuatorsPresenter();
    this->suih = new StateUIHandler(this->stateUI, *this->sp, *this->ap, *this->ac, *this->stc);
    this->sduih = new SystemDiagramUIHandler(this->systemDiagramUI, *this->sp, *this->ap, *this->ac, sensorIds, actuatorIds);
    this->stp = new StatesPresenter(*this->suih);
    this->daqm = new DAQManager(*this->svg);
    // attach presenters to managers (kinda ugly, but idk another way to do it)
    this->sm->setOutputContract(this->sp);
    this->am->setOutputContract(this->ap);
    this->stm->setOutputContract(this->stp);
}

void GSManager::startProcess() {
    this->stm->startProcess();
    this->daqm->startAcquisition();
}

void GSManager::stopProcess() {
    this->daqm->stopAcquisition();
    this->stm->stopProcess();
}

void GSManager::closeProcess() {
    this->stopProcess();
    // these deletes are in the opposite order
    // to the constructs in openProcess().
    delete this->daqm;
    delete this->stp;
    delete this->sp;
    delete this->ap;
    delete this->sduih;
    delete this->suih;
    delete this->stc;
    delete this->ac;
    delete this->svg;
    delete this->stm;
    delete this->am;
    delete this->sm;
    // rerender the UI for the next process.
    this->rerenderUi();
}

void GSManager::renderUi() {
    this->GSMainWindowUI.setupUi(this);
    this->errorUI.setupUi(this->GSMainWindowUI.errorFrame);
    euih.setUI(&this->errorUI);
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
