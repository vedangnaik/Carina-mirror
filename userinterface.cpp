#include "userinterface.h"
#include "ui_userinterface.h"

#include <QFileDialog>

UserInterface::UserInterface(QWidget *parent) : QMainWindow(parent) , ui(new Ui::GSUI) {
    ui->setupUi(this);
//    connect(ui->proceedButton, &QPushButton::click, dynamic_cast<QObject*>(this->pcic), SLOT(this->pcic->proceed()));
//    connect(ui->abortButton, &QPushButton::click, this->pcic, &PCIC::abort);
}

UserInterface::~UserInterface() {
    delete ui;
}

void UserInterface::displayState(State* s) {
    // actually make widgets and whatnot and write to this.ui directly.
}
