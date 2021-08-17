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
}

void GSManager::openProcessFromFile(const std::string& filepath) {
    try {
        // Exceptions will be thrown for any errors in the file format.
        auto t = ProcessFileParser::parseProcessFile(filepath);
        LOG(INFO) << "Process file parsed successfully.";

        // Manufacture all sensors and actuators here first
        std::vector<Sensor*> sensors;
        std::vector<Actuator*> actuators;
        // Reset the factories first
        ConcreteSensorFactory::resetFactory();
        ConcreteActuatorFactory::resetFactory();
        for (const auto& p : std::get<0>(t)) {
            Sensor* s = ConcreteSensorFactory::createSensor(p.first, p.second);
            sensors.push_back(s);
            auto* a = new QAction(QString::fromStdString(p.first));
            connect(a, &QAction::triggered, this, [=]() {
               LOG(INFO) << "User has requested recalibration of sensor '" << s->id << "'.";
               RecalibrationWindow w(s);
               if (w.exec() == QDialog::Accepted) {
                   LOG(INFO) << "Accepted for sensor '" << s->id << "'.";
               } else {
                   LOG(INFO) << "Rejected for sensor '" << s->id << "'.";
               }
            });
            this->GSMainWindowUI.menuRecalibrate_Sensors->addAction(a);
        }
        for (const auto& p : std::get<1>(t)) {
            actuators.push_back(ConcreteActuatorFactory::createActuator(p.first, p.second));
        }

        // First make the presenters which dudes be subscribing to
        this->sp = std::make_unique<SensorsPresenter>();
        this->ap = std::make_unique<ActuatorsPresenter>();
        this->stp = std::make_unique<StatesPresenter>();
        LOG(INFO) << "Presenters ready.";

        // Now initialize the L2 classes with these presenters to use.
        this->sm = std::make_unique<SensorsManager>(sensors, *this->sp);
        this->am = std::make_unique<ActuatorsManager>(actuators, *this->ap);
        this->stm = std::make_unique<StatesManager>(std::get<2>(t), *this->sm, *this->am, *this->stp);
        LOG(INFO) << "Managers ready.";

        // Make the remaining L3 classes here
        this->ac = std::make_unique<ActuatorsController>(*this->am);
        this->stc = std::make_unique<StatesController>(*this->stm);
        LOG(INFO) << "Controllers ready.";

        // Make the L4 UI classes here
        this->suih = std::make_unique<StateUIHandler>(this->stateUI, *this->sp, *this->ap, *this->stp, *this->ac, *this->stc);
        // Array of sensor and actuator IDs for various classes to use.
        std::vector<std::string> sensorIDs, actuatorIDs;
        for (const auto& p : std::get<0>(t)) { sensorIDs.push_back(p.first); }
        for (const auto& p : std::get<1>(t)) { actuatorIDs.push_back(p.first); }
        this->sduih = std::make_unique<SystemDiagramUIHandler>(this->systemDiagramUI, *this->sp, *this->ap, *this->ac, sensorIDs, actuatorIDs);
        LOG(INFO) << "User interface ready.";

        // Enable configuration of DAQs, disable opening new file, enable starting loaded process.
        this->GSMainWindowUI.openProcessFromFileAction->setEnabled(false);
        this->GSMainWindowUI.startProcessAction->setEnabled(true);
    } catch (std::exception& e) {
        LOG(ERROR) << e.what();
    }
}

void GSManager::startProcess() {
    LOG(INFO) << "User has requested process start.";

    this->sm->startAcquisition();
    LOG(INFO) << "DAQManager acquisition started.";
    
    this->stm->startProcess();
    LOG(INFO) << "Process started.";

    this->GSMainWindowUI.startProcessAction->setEnabled(false);
    this->GSMainWindowUI.closeProcessAction->setEnabled(true);
}

void GSManager::stopAndCloseProcess() {
    LOG(INFO) << "User has requested process close.";

    this->sm->stopAcquisition();
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
    Helpers::clearLayout(this->GSMainWindowUI.stateFrame->layout());
    delete this->GSMainWindowUI.stateFrame->layout();
    this->stateUI.setupUi(this->GSMainWindowUI.stateFrame);
    // same as above for systemDiagramFrame.
    Helpers::clearLayout(this->GSMainWindowUI.systemDiagramFrame->layout());
    delete this->GSMainWindowUI.systemDiagramFrame->layout();
    this->systemDiagramUI.setupUi(this->GSMainWindowUI.systemDiagramFrame);
}
