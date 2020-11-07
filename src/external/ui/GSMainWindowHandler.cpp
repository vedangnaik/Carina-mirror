#include "userinterface.h"
#include "ui_GSMainWindow.h"

#include <QFileDialog>
#include <QtUiTools>
#include <QHBoxLayout>
#include <iostream>

GSMainWindowHandler::GSMainWindowHandler(QMainWindow* toHandle, PCIC* pcic) {
    this->toHandle = toHandle;
    this->mainWindowUI = new Ui::GSMainWindow;
    mainWindowUI->setupUi(this->toHandle);
    this->pcic = pcic;

    connect(mainWindowUI->openProcessFromFileAction, &QAction::triggered, this, [this]() {
        QString fileName = QFileDialog::getOpenFileName(this->toHandle,
            tr("Open Process File"), "/home/vedang/Desktop/");
        if (fileName != "") {
            this->pcic->openProcess(fileName.toStdString());
        }
    });
}

GSMainWindowHandler::~GSMainWindowHandler() {
    delete mainWindowUI;
}

QHBoxLayout* GSMainWindowHandler::getAbortButtonLayout() {
    return this->mainWindowUI->abortButtonLayout;
}

QHBoxLayout* GSMainWindowHandler::getProcessSummaryLayout() {
    return this->mainWindowUI->processSummaryLayout;
}

QHBoxLayout* GSMainWindowHandler::getProceedButtonLayout() {
    return this->mainWindowUI->proceedButtonLayout;
}

QVBoxLayout* GSMainWindowHandler::getCurrentStateLayout() {
    return this->mainWindowUI->currentStateLayout;
}
