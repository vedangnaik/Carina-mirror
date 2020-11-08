#include "userinterface.h"
#include "ui_currentState.h"
#include "ui_processSummary.h"

#include <QGroupBox>
#include <iostream>

ProcessUIHandler::ProcessUIHandler(GSMainWindowHandler* gsmwh, PCIC* pcic, ClocksModule* cm) {
    this->stateUI = new Ui::CurrentState;
    stateUI->setupUi(this);
    this->processSummaryUI = new Ui::ProcessSummary;
    processSummaryUI->setupUi(this);
    this->gsmwh = gsmwh;
    this->pcic = pcic;
    this->cm = cm;

    this->abortButton = new QPushButton("Abort");
    this->proceedButton = new QPushButton("Proceed");
    this->gsmwh->getAbortButtonLayout()->addWidget(this->abortButton);
    this->gsmwh->getProceedButtonLayout()->addWidget(this->proceedButton);
    this->gsmwh->getProcessSummaryLayout()->addWidget(this->processSummaryUI->psFrame);
    this->gsmwh->getCurrentStateLayout()->addWidget(this->stateUI->csFrame);

    connect(this->proceedButton, &QPushButton::clicked, this->pcic, &PCIC::proceed);
    this->proceedButton->setEnabled(false);
    connect(this->abortButton, &QPushButton::clicked, this->pcic, &PCIC::abort);
    this->abortButton->setEnabled(false);
}

ProcessUIHandler::~ProcessUIHandler() {
    delete this->stateUI;
    delete this->processSummaryUI;
}

void ProcessUIHandler::allowProceed(bool permission) {
    this->proceedButton->setEnabled(permission);
}

void ProcessUIHandler::allowAbort(bool permission) {
    this->abortButton->setEnabled(permission);
}

void ProcessUIHandler::displayProcessSummary(std::vector<std::string> processSummary) {
    for (std::string summary : processSummary) {
        QGroupBox* summaryBox = new QGroupBox("", this->processSummaryUI->psFrame);
        QLabel* summaryLabel = new QLabel(QString::fromStdString(summary), this->processSummaryUI->psFrame);
        summaryLabel->setWordWrap(true);
        QHBoxLayout* h = new QHBoxLayout();
        h->addWidget(summaryLabel);
        summaryBox->setLayout(h);
        this->processSummaryUI->psLayout->addWidget(summaryBox);
    }
}

/*void ProcessUIHandler::displayState(std::string name, std::string description, std::string abortState, std::map<int, Sensor*> sensorPos, std::map<int, Actuator*> actuatorPos)*/
void ProcessUIHandler::displayState(State* s, std::map<std::string, Actuator*> actuators, std::map<std::string, Sensor*> sensors) {
    QLayoutItem* child;
    while ((child = this->stateUI->actionsLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    this->stateUI->nameLabel->setText(QString::fromStdString(s->name));
    this->stateUI->abortsToLabel->setText(QString::fromStdString(s->abortState));
    this->stateUI->descriptionLabel->setText(QString::fromStdString(s->description));

    unsigned int row = 0;
    for (auto action : s->actions) {
        std::string id = action.first;
        std::vector<unsigned int> options = action.second;
        try {
            Actuator* actuator = actuators.at(id);
            QPushButton* aButton = new QPushButton(QString::fromStdString(id));
            aButton->setCheckable(true);

            for (auto option: options) {
                switch (option) {
                case ActuatorOptions::Timed:
                    QLabel* elapsedTimeLabel = this->timedActuatorHandler(aButton);
                    this->stateUI->actionsLayout->addWidget(elapsedTimeLabel, row, 2);
                    break;
                }
            }

            this->stateUI->actionsLayout->addWidget(new QLabel(QString::fromStdString(id)), row, 0);
            this->stateUI->actionsLayout->addWidget(aButton, row, 1);
        }  catch (std::out_of_range& e) {
            Sensor* sensor = sensors.at(id);
        }

        row += 1;
    }
}

QLabel* ProcessUIHandler::timedActuatorHandler(QPushButton* aButton) {
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
