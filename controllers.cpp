#include "controllers.h"

//ProcessController::ProcessController(QObject *parent) : QObject(parent) {

//}

void ProcessController::proceed() {
    this->pmic->transition("proceed");
}

void ProcessController::abort() {
    this->pmic->transition("abort");
}

//void ProcessController::openProcessFromFile_onClick() {
//    QString fileName = QFileDialog::getOpenFileName(this,
//        tr("Open Process File"), "/home/vedang/Desktop/");
//    if (fileName == "") { return; }

//    ProcessGateway* pg = new ProcessGateway();
//    pg->parseProcessFile(fileName.toStdString(), NULL, this->sm);
//}
