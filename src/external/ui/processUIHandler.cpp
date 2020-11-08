#include "userinterface.h"
#include "ui_currentState.h"
#include "ui_processSummary.h"

#include <QGroupBox>

ProcessUIHandler::ProcessUIHandler(GSMainWindowHandler* gsmwh, PCIC* pcic) {
    this->stateUI = new Ui::CurrentState;
    stateUI->setupUi(this);
    this->processSummaryUI = new Ui::ProcessSummary;
    processSummaryUI->setupUi(this);
    this->gsmwh = gsmwh;
    this->pcic = pcic;

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

    for (auto action : s->actions) {
        std::string id = action.first;
        std::vector<unsigned int> options = action.second;
        try {
            actuators.at(id);
            QPushButton* aButton = new QPushButton(QString::fromStdString(id));
            aButton->setChecked(true);
            // connect(aButton, &QPushButton::toggled, actuator controller, some slot);
        }  catch (std::out_of_range& e) {
            sensors.at(id);
        }
    }

//    for (unsigned long i = 0; i < sensorPos.size() + actuatorPos.size(); i++) {
//        try {
//            Sensor* s = sensorPos.at(i);
//            QLabel* sValueLabel = new QLabel();
//            this->stateUI->actionsLayout->addRow(QString::fromStdString(s->id), sValueLabel);
//        } catch (std::out_of_range& e) {
//            Actuator* a = actuatorPos.at(i);
//            QPushButton* aButton = new QPushButton(QString::fromStdString(a->id));
//            this->stateUI->actionsLayout->addRow(QString::fromStdString(a->id), aButton);
//        }
//    }
}
