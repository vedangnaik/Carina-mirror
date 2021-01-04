#include "GSManager.h"

GSManager::GSManager() {
    this->renderUi();

    LOG(INFO) << "The program has started.";

    connect(this->GSMainWindowUI.openProcessFromFileAction, &QAction::triggered, this, [=]() {
        QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open Process File"), "/", tr("JSON Files (*.json)"));
        if (fileName != "") {
            this->openProcessFromFile(fileName.toStdString());
        }
    });
    connect(this->GSMainWindowUI.startProcessAction, &QAction::triggered, this, &GSManager::startProcess);
    connect(this->GSMainWindowUI.closeProcessAction, &QAction::triggered, this, &GSManager::stopAndCloseProcess);

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

//    connect(this->GSMainWindowUI.actionConfigure_DAQs, &QAction::triggered, this, [=]() {
//        DAQManagerFactory* a = new DAQManagerFactory();
//        a->show();
//    });
}

void GSManager::openProcessFromFile(std::string filepath) {
    // The only use for Process Gateway: to parse the process file here.
    // Exceptions will be thrown for any errors in the file format.
    struct ProcessData pgdata;
    try {
        ProcessGateway pg(filepath);
        pgdata = pg.parseProcessFile();
    } catch (ProcessFileParseError& e) {
        LOG(ERROR) << "Process file parse error:" << e.what();
    }

    // Array of sensor and actuator IDs for the L2 classes
    std::vector<std::string> sensorIds, actuatorIds;
    for (const auto& [id, _] : pgdata.sensors) { sensorIds.push_back(id); }
    for (const auto& [id, _] : pgdata.actuators) { actuatorIds.push_back(id); }
    // Now initialize the L2 classes with them.
    try {
        this->sm = std::make_unique<SensorsManager>(pgdata.sensors);
        this->am = std::make_unique<ActuatorsManager>(pgdata.actuators);
        this->stm = std::make_unique<StatesManager>(pgdata.states, *this->sm, *this->am);
    } catch (SensorsManagerError& e) {
        LOG(ERROR) << "SensorsManager error: " << e.what();
    }

    // init L3 classes here
    this->svg = std::make_unique<SensorValuesGateway>(*this->sm);
    this->ac = std::make_unique<ActuatorsController>(*this->am);
    this->stc = std::make_unique<StatesController>(*this->stm);
    // init L4 and presenters here
    this->sp = std::make_unique<SensorsPresenter>();
    this->ap = std::make_unique<ActuatorsPresenter>();
    this->suih = std::make_unique<StateUIHandler>(this->stateUI, *this->sp, *this->ap, *this->ac, *this->stc);
    this->sduih = std::make_unique<SystemDiagramUIHandler>(this->systemDiagramUI, *this->sp, *this->ap, *this->ac, sensorIds, actuatorIds);
    this->stp = std::make_unique<StatesPresenter>(*this->suih);

    // make the DAQManager here, using the GUI factory.
    this->daqm = std::make_unique<DAQManager>(*this->svg);
    // attach presenters to managers (kinda ugly, but idk another way to do it)
    this->sm->setOutputContract(this->sp.get());
    this->am->setOutputContract(this->ap.get());
    this->stm->setOutputContract(this->stp.get());

    this->GSMainWindowUI.openProcessFromFileAction->setEnabled(false);
    this->GSMainWindowUI.startProcessAction->setEnabled(true);

}

void GSManager::startProcess() {
    this->stm->startProcess();
    this->daqm->startAcquisition();

    this->GSMainWindowUI.startProcessAction->setEnabled(false);
    this->GSMainWindowUI.closeProcessAction->setEnabled(true);
}

void GSManager::stopAndCloseProcess() {
    this->daqm->stopAcquisition();
    this->stm->stopProcess();
    this->rerenderUi();

    this->GSMainWindowUI.closeProcessAction->setEnabled(false);
    this->GSMainWindowUI.openProcessFromFileAction->setEnabled(true);
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
