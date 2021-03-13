#include "GSManager.h"

GSManager::GSManager() {
    this->euih = std::make_unique<ErrorUIHandler>();

    this->renderUi();

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

    connect(this->GSMainWindowUI.manufactureDAQManagerAction, &QAction::triggered, this, [=]() {
        this->daqm = DAQManagerWizard::manufactureDAQManager(this->svg->getSensorIDs());
    });

    connect(this->GSMainWindowUI.reconfigureDAQManagerAction, &QAction::triggered, this, [=]() {
        this->daqm->stopAcquisition();
        this->daqm = DAQManagerWizard::reconfigureDAQManager();
        this->daqm->setOutputContract(this->svg.get());
        this->daqm->startAcquisition();
    });
}

void GSManager::openProcessFromFile(string filepath) {
    try {
        // Exceptions will be thrown for any errors in the file format.
        ProcessFileParser pg(filepath);
        auto t = pg.parseProcessFile();

        // Array of sensor and actuator IDs for various classes to use.
        std::vector<string> sensorIDs, actuatorIDs;
        for (const auto& p : std::get<0>(t)) { sensorIDs.push_back(p.first); }
        for (const auto& p : std::get<1>(t)) { actuatorIDs.push_back(p.first); }

        // First make the presenters which dudes be subscribing to
        this->sp = make_unique<SensorsPresenter>();
        this->ap = make_unique<ActuatorsPresenter>();
        this->stp = make_unique<StatesPresenter>();

        // Now initialize the L2 classes with these presenters to use.
        this->sm = make_unique<SensorsManager>(std::get<0>(t), *this->sp);
        this->am = make_unique<ActuatorsManager>(std::get<1>(t), *this->ap);
        this->stm = make_unique<StatesManager>(std::get<2>(t), *this->sm, *this->am, *this->stp);

        // Make the remaining L3 classes here
        this->svg = make_unique<SensorValuesGateway>(*this->sm);
        this->ac = make_unique<ActuatorsController>(*this->am);
        this->stc = make_unique<StatesController>(*this->stm);

        // Make the L4 UI classes here
        this->suih = make_unique<StateUIHandler>(this->stateUI, *this->sp, *this->ap, *this->stp, *this->ac, *this->stc);
        this->sduih = make_unique<SystemDiagramUIHandler>(this->systemDiagramUI, *this->sp, *this->ap, *this->ac, sensorIDs, actuatorIDs);

        // Enable configuration of DAQs, disable opening new file, enable starting loaded process.
        this->GSMainWindowUI.openProcessFromFileAction->setEnabled(false);
        this->GSMainWindowUI.startProcessAction->setEnabled(true);
        this->GSMainWindowUI.manufactureDAQManagerAction->setEnabled(true);
        this->GSMainWindowUI.reconfigureDAQManagerAction->setEnabled(false);
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
    if (this->daqm == nullptr) {
        LOG(ERROR) << "Please configure your DAQ devices first.";
        return;
    }
    this->daqm->setOutputContract(this->svg.get());
    this->daqm->startAcquisition();
    this->stm->startProcess();

    this->GSMainWindowUI.startProcessAction->setEnabled(false);
    this->GSMainWindowUI.closeProcessAction->setEnabled(true);
    this->GSMainWindowUI.manufactureDAQManagerAction->setEnabled(false);
    this->GSMainWindowUI.reconfigureDAQManagerAction->setEnabled(true);
}

void GSManager::stopAndCloseProcess() {
    if (this->daqm != nullptr) this->daqm->stopAcquisition();
    this->stm->stopProcess();
    this->rerenderUi();

    this->GSMainWindowUI.closeProcessAction->setEnabled(false);
    this->GSMainWindowUI.openProcessFromFileAction->setEnabled(true);
    this->GSMainWindowUI.manufactureDAQManagerAction->setEnabled(false);
    this->GSMainWindowUI.reconfigureDAQManagerAction->setEnabled(false);
}

void GSManager::renderUi() {
    this->GSMainWindowUI.setupUi(this);
    this->errorUI.setupUi(this->GSMainWindowUI.errorFrame);
    this->euih->setUI(&this->errorUI);
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
