#include "GSManager.h"

GSManager::GSManager() {
    this->renderUi();

    // Attach the error dialog right now, to show logs as early as possible.
    this->euih = std::make_unique<ErrorUIHandler>(this->errorUI);
    LOG(INFO) << "Logging is ready.";

    connect(this->GSMainWindowUI.openProcessFromFileAction, &QAction::triggered, this, [=]() {
        LOG(INFO) << "User has requested process file open.";

        QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open Process File"), "/", tr("JSON Files (*.json)"));
        if (fileName != "") {
            this->openProcessFromFile(fileName.toStdString());
            LOG(INFO) << "Opened process file: " << fileName.toStdString();
        }
    });

    connect(this->GSMainWindowUI.startProcessAction, &QAction::triggered, this, &GSManager::startProcess);
    connect(this->GSMainWindowUI.closeProcessAction, &QAction::triggered, this, &GSManager::stopAndCloseProcess);

    connect(this->GSMainWindowUI.openSystemDiagramAction, &QAction::triggered, this, [=]() {
        QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open System Diagram Image"), "/", tr("Image Files (*.png *.jpg)"));
        if (fileName != "") {
            this->systemDiagramUI.systemDiagramFrame->setStyleSheet("#systemDiagramFrame{ border-image: url(" + fileName + ") 0 0 0 0 }");
            LOG(INFO) << "Added system diagram file: " << fileName.toStdString();
        }
    });
    connect(this->GSMainWindowUI.clearSystemDiagramAction, &QAction::triggered, this, [=]() {
        this->systemDiagramUI.systemDiagramFrame->setStyleSheet("");
        LOG(INFO) << "Removed system diagram file.";
    });

    connect(this->GSMainWindowUI.manufactureDAQManagerAction, &QAction::triggered, this, [=]() {
        LOG(INFO) << "User has requested DAQManager configuration.";

        if (this->svg != nullptr) {
            DAQScanDialog* d = new DAQScanDialog();
            if (d->exec() == QDialog::Accepted) {
                this->daqm = std::make_unique<DAQManager>(d->DAQDevices, *this->svg);
                LOG(INFO) << "New DAQManager manufactured.";
            } else {
                LOG(INFO) << "DAQManager manufacturing cancelled.";
            };
        } else {
            LOG(ERROR) << "Please load a process first.";
        }
    });

    connect(this->GSMainWindowUI.calibrateDAQManagerAction, &QAction::triggered, this, [=]() {
        LOG(INFO) << "User has requested DAQManager recalibration.";

        if (this->daqm != nullptr) {
            // Create a new calibration dialog window and move the existing DAQManager into it.
            DAQCalibrationDialog* d = new DAQCalibrationDialog(std::move(this->daqm));
            d->exec();
            // Take it back when done, then log this.
            this->daqm = d->takeDAQManager();
            LOG(INFO) << "DAQManager recalibrated.";
        } else {
            LOG(ERROR) << "Please configure some DAQ devices first.";
        }
    });

    connect(this->GSMainWindowUI.linkDAQManagerAction, &QAction::triggered, this, [=]() {
        LOG(INFO) << "User has requested DAQManager re-linking.";

        if (this->daqm != nullptr) {
            // Same deal here as for calibration.
            DAQLinkDialog* d = new DAQLinkDialog(std::move(this->daqm));
            d->exec();
            this->daqm = d->takeDAQManager();
            LOG(INFO) << "DAQManager re-linked.";
        } else {
            LOG(ERROR) << "Please configure some DAQ devices first.";
        }
    });
}

void GSManager::openProcessFromFile(string filepath) {
    try {
        // Exceptions will be thrown for any errors in the file format.
        ProcessFileParser pg(filepath);
        auto t = pg.parseProcessFile();
        LOG(INFO) << "Process file parsed successfully.";

        // Array of sensor and actuator IDs for various classes to use.
        std::vector<string> sensorIDs, actuatorIDs;
        for (const auto& p : std::get<0>(t)) { sensorIDs.push_back(p.first); }
        for (const auto& p : std::get<1>(t)) { actuatorIDs.push_back(p.first); }

        // First make the presenters which dudes be subscribing to
        this->sp = make_unique<SensorsPresenter>();
        this->ap = make_unique<ActuatorsPresenter>();
        this->stp = make_unique<StatesPresenter>();
        LOG(INFO) << "Presenters ready.";

        // Now initialize the L2 classes with these presenters to use.
        this->sm = make_unique<SensorsManager>(std::get<0>(t), *this->sp);
        this->am = make_unique<ActuatorsManager>(std::get<1>(t), *this->ap);
        this->stm = make_unique<StatesManager>(std::get<2>(t), *this->sm, *this->am, *this->stp);
        LOG(INFO) << "Managers ready.";

        // Make the remaining L3 classes here
        this->svg = make_unique<SensorValuesGateway>(*this->sm);
        this->ac = make_unique<ActuatorsController>(*this->am);
        this->stc = make_unique<StatesController>(*this->stm);
        LOG(INFO) << "Controllers ready.";

        // Make the L4 UI classes here
        this->suih = make_unique<StateUIHandler>(this->stateUI, *this->sp, *this->ap, *this->stp, *this->ac, *this->stc);
        this->sduih = make_unique<SystemDiagramUIHandler>(this->systemDiagramUI, *this->sp, *this->ap, *this->ac, sensorIDs, actuatorIDs);
        LOG(INFO) << "User interface ready.";

        // Enable configuration of DAQs, disable opening new file, enable starting loaded process.
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
    LOG(INFO) << "User has requested process start.";

    if (this->daqm == nullptr) {
        LOG(ERROR) << "Please configure your DAQ devices first.";
        return;
    }
    this->daqm->startAcquisition();
    LOG(INFO) << "DAQManager acquisition started.";
    
    this->stm->startProcess();
    LOG(INFO) << "Process started.";

    this->GSMainWindowUI.startProcessAction->setEnabled(false);
    this->GSMainWindowUI.closeProcessAction->setEnabled(true);
}

void GSManager::stopAndCloseProcess() {
    LOG(INFO) << "User has requested process close.";

    this->daqm->stopAcquisition();
    LOG(INFO) << "DAQManager acquisition stopped.";
    
    this->stm->stopProcess();
    LOG(INFO) << "Process closed.";
    
    this->rerenderUi();
    this->GSMainWindowUI.closeProcessAction->setEnabled(false);
    this->GSMainWindowUI.openProcessFromFileAction->setEnabled(true);
}

void GSManager::renderUi() {
    this->GSMainWindowUI.setupUi(this);
    this->errorUI.setupUi(this->GSMainWindowUI.errorFrame);
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
