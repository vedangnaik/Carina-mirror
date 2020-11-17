#include "StateUIHandler.h"

StateUIHandler::StateUIHandler(GSMainWindowHandler* gsmwh, ACIC* acic, SMIC* smic, ClocksModule* cm) {
    this->stateUI = new Ui::State;
    stateUI->setupUi(this);
    gsmwh->getCurrentStateLayout()->addWidget(this->stateUI->csFrame);
    this->acic = acic;
    this->smic = smic;
    this->cm = cm;
}

StateUIHandler::~StateUIHandler() {
    delete this->stateUI;
}

void StateUIHandler::displayState(const State& s) {
    QLayoutItem* child;
    while ((child = this->stateUI->actionsLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    this->stateUI->nameLabel->setText(QString::fromStdString(s.name));
    this->stateUI->abortLabel->setText(QString::fromStdString(s.transitions.at(Transition::Abort)));
    this->stateUI->nextLabel->setText(QString::fromStdString(s.transitions.at(Transition::Proceed)));
    this->stateUI->descriptionLabel->setText(QString::fromStdString(s.description));
    this->stateUI->csFrame->setStyleSheet("#csFrame {border: 4px solid " + QString::fromStdString(s.safetyRating) + "}");

    unsigned int row = 0;
    for (std::string id : s.actionsOrder) {
        if (s.actuatorOptions.find(id) != s.actuatorOptions.end()) {
            QPushButton* aButton = new QPushButton(QString::fromStdString(id));
            aButton->setCheckable(true);
            connect(aButton, &QPushButton::toggled, this->acic, [=]() {
                this->acic->actuate(id);
            });

            this->stateUI->actionsLayout->addWidget(new QLabel(QString::fromStdString(id)), row, 0);
            this->stateUI->actionsLayout->addWidget(aButton, row, 1);

            for (ActuatorOption o: s.actuatorOptions.at(id)) {
                switch (o) {
                case ActuatorOption::Timed:
                    this->stateUI->actionsLayout->addWidget(this->timedActuatorHandler(aButton), row, 2);
                    break;
                case ActuatorOption::Automatic:
                    aButton->toggle();
                    break;
                case ActuatorOption::None:
                    break;
                }
            }
        } else if (s.sensorOptions.find(id) != s.sensorOptions.end()) {
            QLabel* sensorValueLabel = new QLabel();
            // not strictly best practice to know the manager directly, but hey
            connect(this->cm->HundredMsTimer, &QTimer::timeout, sensorValueLabel, [=]() {
                sensorValueLabel->setText(QString::number(this->smic->getSensorValue(id)));
            });

            this->stateUI->actionsLayout->addWidget(new QLabel(QString::fromStdString(id)), row, 0);
            this->stateUI->actionsLayout->addWidget(sensorValueLabel, row, 1);
            for (SensorOption o: s.sensorOptions.at(id)) {
                switch (o) {
                case SensorOption::None:
                    break;
                }
            }
        } else {
            // shit
        }

        row += 1;
    }
}


QLabel* StateUIHandler::timedActuatorHandler(QPushButton* aButton) {
    QLabel* elapsedTimeLabel = new QLabel();
    connect(aButton, &QPushButton::toggled, elapsedTimeLabel, [=](bool checked) {
        if (checked) {
            connect(this->cm->oneSTimer, &QTimer::timeout, elapsedTimeLabel, [=]() {
                int elapsedTime = elapsedTimeLabel->text().toInt() + 1;
                elapsedTimeLabel->setText(QString::number(elapsedTime));
            });
        } else {
            disconnect(this->cm->oneSTimer, &QTimer::timeout, elapsedTimeLabel, nullptr);
            elapsedTimeLabel->setText("");
        }
    });
    return elapsedTimeLabel;
}
