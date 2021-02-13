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

    connect(this->GSMainWindowUI.actionConfigure_DAQ_Devices, &QAction::triggered, this, [=]() {
        this->daqm = DAQManagerWizard::manufactureDAQManager(std::move(this->daqm), this->svg->getSensorIDs());
    });

    connect(this->GSMainWindowUI.actionRecalibrate_DAQ_Devices, &QAction::triggered, this, [=]() {
       this->daqm = DAQManagerWizard::recalibrateDAQs(std::move(this->daqm));
    });

    connect(this->GSMainWindowUI.actionRe_link_sensors_and_channels, &QAction::triggered, this, [=]() {
       this->daqm = DAQManagerWizard::relinkSensors(std::move(this->daqm), this->svg->getSensorIDs());
    });
}

void GSManager::openProcessFromFile(string filepath) {
    // Exceptions will be thrown for any errors in the file format.
    try {
        ProcessFileParser pg(filepath);
        auto t = pg.parseProcessFile();

        // Array of sensor and actuator IDs for the L2 classes
        std::vector<string> sensorIDs, actuatorIDs;
        for (const auto& p : std::get<0>(t)) { sensorIDs.push_back(p.first); }
        for (const auto& p : std::get<1>(t)) { actuatorIDs.push_back(p.first); }
        // Now initialize the L2 classes with them.
        this->sm = make_unique<SensorsManager>(std::get<0>(t));
        this->am = make_unique<ActuatorsManager>(std::get<1>(t));
        this->stm = make_unique<StatesManager>(std::get<2>(t), *this->sm, *this->am);

        // init L3 classes here
        this->svg = make_unique<SensorValuesGateway>(*this->sm);
        this->ac = make_unique<ActuatorsController>(*this->am);
        this->stc = make_unique<StatesController>(*this->stm);
        // init L4 and presenters here
        this->sp = make_unique<SensorsPresenter>();
        this->ap = make_unique<ActuatorsPresenter>();
        this->suih = make_unique<StateUIHandler>(this->stateUI, *this->sp, *this->ap, *this->ac, *this->stc);
        this->sduih = make_unique<SystemDiagramUIHandler>(this->systemDiagramUI, *this->sp, *this->ap, *this->ac, sensorIDs, actuatorIDs);
        this->stp = make_unique<StatesPresenter>(*this->suih);

        this->GSMainWindowUI.actionConfigure_DAQ_Devices->setEnabled(true);

        // attach presenters to managers (kinda ugly, but idk another way to do it)
        this->sm->setOutputContract(this->sp.get());
        this->am->setOutputContract(this->ap.get());
        this->stm->setOutputContract(this->stp.get());

        this->GSMainWindowUI.openProcessFromFileAction->setEnabled(false);
        this->GSMainWindowUI.startProcessAction->setEnabled(true);

    } catch (ProcessFileParseError& e) {
        LOG(ERROR) << "Process file parse error:" << e.what();
    } catch (SensorsManagerError& e) {
        LOG(ERROR) << "SensorsManager error: " << e.what();
    } catch(StatesManagerError& e){
        LOG(ERROR) << "StatesManager error: " << e.what();
    }
}

void GSManager::startProcess() {
    // TODO: Add the wizard invocation here if it's nullptr
    this->daqm->setOutputContract(this->svg.get());
    this->daqm->startAcquisition();
    this->stm->startProcess();

    this->GSMainWindowUI.startProcessAction->setEnabled(false);
    this->GSMainWindowUI.closeProcessAction->setEnabled(true);
}

void GSManager::stopAndCloseProcess() {
    if (this->daqm != nullptr) this->daqm->stopAcquisition();
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
