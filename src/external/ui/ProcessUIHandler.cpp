#include "userinterface.h"
#include "ui_processsummary.h"

#include <QGroupBox>
#include <iostream>

ProcessUIHandler::ProcessUIHandler(GSMainWindowHandler* gsmwh, StateUIHandler* suih, PCIC* pcic) {
    this->processSummaryUI = new Ui::ProcessSummary;
    processSummaryUI->setupUi(this);
    this->suih = suih;
    this->pcic = pcic;

    this->abortButton = new QPushButton("Abort");
    this->proceedButton = new QPushButton("Proceed");
    gsmwh->getAbortButtonLayout()->addWidget(this->abortButton);
    gsmwh->getProceedButtonLayout()->addWidget(this->proceedButton);
    gsmwh->getProcessSummaryLayout()->addWidget(this->processSummaryUI->psFrame);

    connect(this->proceedButton, &QPushButton::clicked, this->pcic, &PCIC::proceed);
    this->proceedButton->setEnabled(false);
    connect(this->abortButton, &QPushButton::clicked, this->pcic, &PCIC::abort);
    this->abortButton->setEnabled(false);
}

ProcessUIHandler::~ProcessUIHandler() {
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


void ProcessUIHandler::displayState(State* s, std::map<std::string, Actuator*> actuators, std::map<std::string, Sensor*> sensors) {
    this->suih->displayState(s, actuators, sensors);
}
