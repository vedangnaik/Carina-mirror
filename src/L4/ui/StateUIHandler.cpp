#include "StateUIHandler.h"

// forward declared helpers
QLabel* displaySensorCheck(const SensorCheck& sc);
QLabel* displayActuatorCheck(const ActuatorCheck& ac);

StateUIHandler::StateUIHandler(Ui::State& stateUI, ACIC& acic, StCIC& stcic, ClocksModule& cm) : stateUI(stateUI), acic(acic), stcic(stcic), cm(cm) {
    connect(this->stateUI.proceedButton, &QPushButton::clicked, &this->stcic, &StCIC::proceed);
    connect(this->stateUI.abortButton, &QPushButton::clicked, &this->stcic, &StCIC::abort);
};

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
            QPushButton* aButton = new QPushButton(QString::fromStdString(id));
            aButton->setCheckable(true);
            connect(aButton, &QPushButton::toggled, &this->acic, [=]() {
                this->acic.actuate(id);
            });

            this->stateUI.actionsLayout->addWidget(new QLabel(QString::fromStdString(id)), row, 0);
            this->stateUI.actionsLayout->addWidget(aButton, row, 1);

            for (ActuatorOption o: s.actuatorOptions.at(id)) {
                switch (o) {
                case ActuatorOption::Timed:
                    this->stateUI.actionsLayout->addWidget(this->displayTimedActuator(aButton), row, 4);
                    break;
                case ActuatorOption::Automatic:
                    aButton->toggle();
                    break;
                }
            }
        } else if (s.sensorOptions.find(id) != s.sensorOptions.end()) {
            QLabel* sensorValueLabel = new QLabel();
            this->subscribe(id, sensorValueLabel);

            this->stateUI.actionsLayout->addWidget(new QLabel(QString::fromStdString(id)), row, 0);
            this->stateUI.actionsLayout->addWidget(sensorValueLabel, row, 1);
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

void StateUIHandler::displaySensorValue(const std::string id, const float value) {
    if (this->sensorDisplaySubscribers.find(id) != this->sensorDisplaySubscribers.end()) {
        this->sensorDisplaySubscribers.at(id)->setText(QString::number(value));
    }
}

QLabel* StateUIHandler::displayTimedActuator(QPushButton* aButton) {
    QLabel* elapsedTimeLabel = new QLabel();
    connect(aButton, &QPushButton::toggled, elapsedTimeLabel, [=](bool checked) {
        if (checked) {
            connect(this->cm.oneSTimer, &QTimer::timeout, elapsedTimeLabel, [=]() {
                int elapsedTime = elapsedTimeLabel->text().toInt() + 1;
                elapsedTimeLabel->setText(QString::number(elapsedTime));
            });
        } else {
            disconnect(this->cm.oneSTimer, &QTimer::timeout, elapsedTimeLabel, nullptr);
            elapsedTimeLabel->setText("");
        }
    });
    return elapsedTimeLabel;
}

void StateUIHandler::subscribe(std::string id, QLabel* label) {
    this->cm.stop();
    this->sensorDisplaySubscribers.insert(std::make_pair(id, label));
    this->cm.start();
    connect(label, &QLabel::destroyed, this, [=]() {
        this->cm.stop();
        this->sensorDisplaySubscribers.erase(id);
        this->cm.start();
    });
}




QLabel* displaySensorCheck(const SensorCheck& sc) {
    return new QLabel("[" + QString::number(sc.a) + ", " + QString::number(sc.b) + "]");
}

QLabel* displayActuatorCheck(const ActuatorCheck& ac) {
    return new QLabel(QVariant(ac.status).toString());
}
