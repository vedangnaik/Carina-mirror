#include "userinterface.h"
#include "ui_userinterface.h"

#include <QFileDialog>

UserInterface::UserInterface(PCIC* pcic, QWidget *parent) : QMainWindow(parent) , ui(new Ui::GSUI) {
    ui->setupUi(this);
    this->pcic = pcic;
    connect(ui->proceedButton, &QPushButton::clicked, this->pcic, &PCIC::proceed);
}

UserInterface::~UserInterface() {
    delete ui;
}

void UserInterface::displayState(State* s) {
    // actually make widgets and whatnot and write to this.ui directly.
}
