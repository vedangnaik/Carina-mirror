#include "userinterface.h"
#include "ui_userinterface.h"

#include <QFileDialog>

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
}

UserInterface::~UserInterface() {
    delete ui;
}

void UserInterface::displayState(State* s) {
    // actually make widgets and whatnot and write to this.ui directly.
}
