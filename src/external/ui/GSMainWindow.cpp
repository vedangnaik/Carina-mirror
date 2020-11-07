#include "userinterface.h"
#include "ui_GSMainWIndow.h"

#include <QFileDialog>
#include <QtUiTools>
#include <QHBoxLayout>
#include <iostream>

GSMainWindow::GSMainWindow(PCIC* pcic, QWidget *parent) : QMainWindow(parent) , ui(new Ui::GSMainWindow) {
    ui->setupUi(this);
    this->pcic = pcic;

    connect(ui->openProcessFromFileAction, &QAction::triggered, this, [this]() {
        QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open Process File"), "/home/vedang/Desktop/");
        if (fileName != "") {
            this->pcic->openProcess(fileName.toStdString());
        }
    });

    connect(ui->proceedButton, &QPushButton::clicked, this->pcic, &PCIC::proceed);
    ui->proceedButton->setEnabled(false);
    connect(ui->abortButton, &QPushButton::clicked, this->pcic, &PCIC::abort);
    ui->abortButton->setEnabled(false);
}

GSMainWindow::~GSMainWindow() {
    delete ui;
}

void GSMainWindow::displayState(
        std::string name,
        std::string description,
        std::string abortState,
        std::map<int, Sensor*> sensorPos,
        std::map<int, Actuator*> actuatorPos
    ) {
    QLayoutItem* child;
    while ((child = ui->currentStateLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    QFile file("src/external/ui/currentState.ui");
    file.open(QIODevice::ReadOnly);
    QUiLoader loader;
    QWidget* stateFrame = loader.load(&file, this);
    file.close();

    QLabel* nameLabel = stateFrame->findChild<QLabel*>("nameLabel");
    nameLabel->setText(QString::fromStdString(name));
    QLabel* descriptionLabel = stateFrame->findChild<QLabel*>("descriptionLabel");
    descriptionLabel->setText(QString::fromStdString(description));
    QLabel* abortsToLabel = stateFrame->findChild<QLabel*>("abortsToLabel");
    abortsToLabel->setText(QString::fromStdString(abortState));

    QFormLayout* fl = new QFormLayout;
    for (unsigned long i = 0; i < sensorPos.size() + actuatorPos.size(); i++) {
        try {
            Sensor* s = sensorPos.at(i);
            QLabel* sValueLabel = new QLabel(stateFrame);
            fl->addRow(QString::fromStdString(s->id), sValueLabel);
        } catch (std::out_of_range& e) {
            Actuator* a = actuatorPos.at(i);
            QPushButton* aButton = new QPushButton(QString::fromStdString(a->id), stateFrame);
            fl->addRow(QString::fromStdString(a->id), aButton);
        }
    }
    QWidget* actionsWidget =  stateFrame->findChild<QWidget*>("actionsWidget");
    actionsWidget->setLayout(fl);

    ui->currentStateLayout->addWidget(stateFrame);
}

void GSMainWindow::displayProcessSummary(std::vector<std::string> processSummary) {
    QVBoxLayout* vl = new QVBoxLayout();
    for (std::string summary : processSummary) {
        QGroupBox* summaryBox = new QGroupBox("", ui->currentProcessScrollArea);
        QLabel* summaryLabel = new QLabel(QString::fromStdString(summary), ui->currentProcessScrollArea);
        summaryLabel->setWordWrap(true);
        QHBoxLayout* h = new QHBoxLayout();
        h->addWidget(summaryLabel);
        summaryBox->setLayout(h);
        vl->addWidget(summaryBox);
    }
    ui->currentProcessScrollArea->setLayout(vl);
}

void GSMainWindow::allowProceed(bool permission) {
    ui->proceedButton->setEnabled(permission);
}

void GSMainWindow::allowAbort(bool permission) {
    ui->abortButton->setEnabled(permission);
}
