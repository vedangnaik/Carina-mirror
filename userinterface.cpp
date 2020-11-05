#include "userinterface.h"
#include "ui_userinterface.h"

#include <QFileDialog>
#include <QtUiTools>
#include <QHBoxLayout>
#include <iostream>

UserInterface::UserInterface(PCIC* pcic, QWidget *parent) : QMainWindow(parent) , ui(new Ui::GSUI) {
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
}

UserInterface::~UserInterface() {
    delete ui;
}

void UserInterface::displayState(State* s) {
    QLayoutItem* child;
    while ((child = ui->currentStateLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    QFile file("currentState.ui");
    file.open(QIODevice::ReadOnly);
    QUiLoader loader;
    QWidget* stateFrame = loader.load(&file, this);
    file.close();

    QLabel* nameLabel = findChild<QLabel*>("nameLabel");
    nameLabel->setText(QString::fromStdString(s->name));
    QLabel* descriptionLabel = findChild<QLabel*>("descriptionLabel");
    descriptionLabel->setText(QString::fromStdString(s->description));
    QLabel* abortsToLabel = findChild<QLabel*>("abortsToLabel");
    abortsToLabel->setText(QString::fromStdString(s->abortState));

    ui->currentStateLayout->addWidget(stateFrame);
}
