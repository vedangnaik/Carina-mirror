#include "StateUIHandler.h"

StateUIHandler::StateUIHandler(GSMainWindowHandler* gsmwh, ACIC* acic, ClocksModule* cm) {
    this->stateUI = new Ui::State;
    stateUI->setupUi(this);
    gsmwh->getCurrentStateLayout()->addWidget(this->stateUI->csFrame);
    this->acic = acic;
    this->cm = cm;
}

StateUIHandler::~StateUIHandler() {
    delete this->stateUI;
}

void StateUIHandler::displayState(StateDisplayInfo sdi) {
    QLayoutItem* child;
    while ((child = this->stateUI->actionsLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    this->stateUI->nameLabel->setText(QString::fromStdString(sdi.name));
    this->stateUI->abortsToLabel->setText(QString::fromStdString(sdi.abortState));
    this->stateUI->descriptionLabel->setText(QString::fromStdString(sdi.description));
    // change colour for state saftey colour here

    unsigned int row = 0;
    for (std::string id : sdi.actionsOrder) {
        if (sdi.actuatorOptions.find(id) != sdi.actuatorOptions.end()) {
            QPushButton* aButton = new QPushButton(QString::fromStdString(id));
            aButton->setCheckable(true);
            connect(aButton, &QPushButton::toggled, this->acic, [=]() {
                this->acic->actuate(id);
            });

            for (ActuatorOption o: sdi.actuatorOptions[id]) {
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

            this->stateUI->actionsLayout->addWidget(new QLabel(QString::fromStdString(id)), row, 0);
            this->stateUI->actionsLayout->addWidget(aButton, row, 1);
        } else if (sdi.sensorOptions.find(id) != sdi.sensorOptions.end()) {
            // make a label and connect to timer to display i guess
            for (SensorOption o: sdi.sensorOptions[id]) {
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
