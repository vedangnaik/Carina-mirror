#include "presenters.h"
#include "ui_gsui.h"

#include <QFileDialog>

ProcessPresenter::ProcessPresenter(QWidget *parent) : QMainWindow(parent) , ui(new Ui::GSUI) {
    ui->setupUi(this);

    connect(ui->proceedButton, &QPushButton::click, this, &ProcessPresenter::proceedButton_onClick);
    connect(ui->openProcessFromFileAction, &QAction::triggered, this, &ProcessPresenter::openProcessFromFile_onClick);
//    connect(ui->openProcessFromFileAction, &QAction::trigger, this, &ProcessPresenter::openProcessFromFile_onClick);

}

ProcessPresenter::~ProcessPresenter() {
    delete ui;
}

void ProcessPresenter::proceedButton_onClick() {

}

void ProcessPresenter::openProcessFromFile_onClick() {
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Process File"), "/home/vedang/Desktop/");
    if (fileName == "") { return; }

    ProcessGateway* pg = new ProcessGateway();
    pg->parseProcessFile(fileName.toStdString(), NULL, this->sm);
}
