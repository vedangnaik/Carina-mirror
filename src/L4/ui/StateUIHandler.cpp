#include "StateUIHandler.h"

// forward declared helpers
QLabel* displaySensorCheck(const SensorCheck& sc);
QLabel* displayActuatorCheck(const ActuatorCheck& ac);

StateUIHandler::StateUIHandler(Ui::State& stateUI, SPIC& spic, APIC& apic, StPIC& stpic, ACIC& acic, StCIC& stcic)
    : stateUI{stateUI}, spic{spic}, apic{apic}, stpic{stpic}, acic{acic}, stcic{stcic}
{
    // subscrivbe yourself to the presenter asap.
    this->stpic.subscribe(this);
    this->actuatorButtonTimer = new QTimer(this);
    this->actuatorButtonTimer->start(1000);
    // this->actuatorButtonTimer->stop() should happen automatically on delete.
    connect(this->stateUI.proceedButton, &QPushButton::clicked, &this->stcic, &StCIC::proceed);
    connect(this->stateUI.abortButton, &QPushButton::clicked, &this->stcic, &StCIC::abort);
}

void StateUIHandler::displayState(const State& s) {
    uihelpers::clearLayout(this->stateUI.actionsLayout);

    this->stateUI.nameLabel->setText(QString::fromStdString(s.name));
    this->stateUI.abortLabel->setText(QString::fromStdString(s.transitions.at(Transition::Abort)));
    this->stateUI.nextLabel->setText(QString::fromStdString(s.transitions.at(Transition::Proceed)));
    this->stateUI.descriptionLabel->setText(QString::fromStdString(s.description));
    this->stateUI.csFrame->setStyleSheet("#csFrame {border: 4px solid " + QString::fromStdString(s.safetyRating) + "}");

    this->stateUI.actionsLayout->addWidget(new QLabel("Label"), 0, 0);
    this->stateUI.actionsLayout->addWidget(new QLabel("Action"), 0, 1);
    this->stateUI.actionsLayout->addWidget(new QLabel("Proceed Check"), 0, 2);
    this->stateUI.actionsLayout->addWidget(new QLabel("Abort Check"), 0, 3);
    this->stateUI.actionsLayout->addWidget(new QLabel("Actuation Timer"), 0, 4);

    unsigned int row = 1;
    for (std::string id : s.actionsOrder) {
        if (s.actuatorOptions.find(id) != s.actuatorOptions.end()) {
            StateUI::ActuatorButton* ab = new StateUI::ActuatorButton();
            this->apic.subscribe(id, ab);
            ab->setCheckable(true);
            ab->setText(QString::fromStdString(id));
            connect(ab, &QPushButton::clicked, &this->acic, [=]() {
                this->acic.actuate(id);
            });
            connect(ab, &QPushButton::destroyed, this, [=]() {
                this->apic.unsubscribe(id, ab);
            });

            this->stateUI.actionsLayout->addWidget(new QLabel(QString::fromStdString(id)), row, 0);
            this->stateUI.actionsLayout->addWidget(ab, row, 1);

            for (ActuatorOption o: s.actuatorOptions.at(id)) {
                switch (o) {
                case ActuatorOption::Timed:
                    this->stateUI.actionsLayout->addWidget(this->displayTimedActuator(ab), row, 4);
                    break;
                case ActuatorOption::Automatic:
                    ab->click();
                    break;
                }
            }
        } else if (s.sensorOptions.find(id) != s.sensorOptions.end()) {
            StateUI::SensorDisplayLabel* svl = new StateUI::SensorDisplayLabel();
            this->spic.subscribe(id, svl);
            connect(svl, &QLabel::destroyed, this, [=]() {
                this->spic.unsubscribe(id, svl);
            });

            this->stateUI.actionsLayout->addWidget(new QLabel(QString::fromStdString(id)), row, 0);
            this->stateUI.actionsLayout->addWidget(svl, row, 1);
            for (SensorOption o: s.sensorOptions.at(id)) {
                switch (o) {
                    // switch for sensor options here
                }
            }
        } else {
            // shit
        }

        const auto& proceedActuatorChecks = s.actuatorChecks.at(Transition::Proceed);
        const auto& abortActuatorChecks = s.actuatorChecks.at(Transition::Abort);
        if (proceedActuatorChecks.find(id) != proceedActuatorChecks.end()) {
            this->stateUI.actionsLayout->addWidget(displayActuatorCheck(proceedActuatorChecks.at(id)), row, 2);
        }
        if (abortActuatorChecks.find(id) != abortActuatorChecks.end()) {
            this->stateUI.actionsLayout->addWidget(displayActuatorCheck(abortActuatorChecks.at(id)), row, 3);
        }

        const auto& proceedSensorChecks = s.sensorChecks.at(Transition::Proceed);
        const auto& abortSensorChecks = s.sensorChecks.at(Transition::Abort);
        if (proceedSensorChecks.find(id) != proceedSensorChecks.end()) {
            this->stateUI.actionsLayout->addWidget(displaySensorCheck(proceedSensorChecks.at(id)), row, 2);
        }
        if (abortSensorChecks.find(id) != abortSensorChecks.end()) {
            this->stateUI.actionsLayout->addWidget(displaySensorCheck(abortSensorChecks.at(id)), row, 3);
        }

        row += 1;
    }
}

void StateUIHandler::displayProcessSummary(const std::vector<std::string> processSummary) {
    uihelpers::clearLayout(this->stateUI.psLayout);
    for (const std::string& summary : processSummary) {
        QGroupBox* summaryBox = new QGroupBox("", this->stateUI.psFrame);
        QLabel* summaryLabel = new QLabel(QString::fromStdString(summary), this->stateUI.psFrame);
        summaryLabel->setWordWrap(true);
        QHBoxLayout* h = new QHBoxLayout();
        h->addWidget(summaryLabel);
        summaryBox->setLayout(h);
        this->stateUI.psLayout->addWidget(summaryBox);
    }
}

void StateUIHandler::allowProceed(bool permission) {
    this->stateUI.proceedButton->setEnabled(permission);
}

void StateUIHandler::allowAbort(bool permission) {
    this->stateUI.abortButton->setEnabled(permission);
}

void
StateUIHandler::allowFailedChecksOverride(const std::vector<std::string> failures, Transition t)
{
    QString err = "The following checks have failed:\n";
    for (const auto& e : failures) {
        err += QString::fromStdString("\t" + e + "\n");
    }
    err += "Do you wish to " + (t == Proceed ? QString::fromStdString("proceed?") : QString::fromStdString("abort?"));
    int ret = QMessageBox::warning(this, tr("Carina"), err, QMessageBox::Ok | QMessageBox::Cancel);

    if (ret == QMessageBox::Ok) {
        t == Proceed ? this->stcic.proceed(true) : this->stcic.abort(true);
    }
}

QLabel* StateUIHandler::displayTimedActuator(QPushButton* aButton) {
    QLabel* elapsedTimeLabel = new QLabel();
    connect(aButton, &QPushButton::toggled, elapsedTimeLabel, [=](bool checked) {
        if (checked) {
            connect(this->actuatorButtonTimer, &QTimer::timeout, elapsedTimeLabel, [=]() {
                int elapsedTime = elapsedTimeLabel->text().toInt() + 1;
                elapsedTimeLabel->setText(QString::number(elapsedTime));
            });
        } else {
            disconnect(this->actuatorButtonTimer, &QTimer::timeout, elapsedTimeLabel, nullptr);
            elapsedTimeLabel->setText("");
        }
    });
    return elapsedTimeLabel;
}



QLabel* displaySensorCheck(const SensorCheck& sc) {
    return new QLabel("[" + QString::number(sc.a) + ", " + QString::number(sc.b) + "]");
}

QLabel* displayActuatorCheck(const ActuatorCheck& ac) {
    return new QLabel(QVariant(ac.status).toString());
}
