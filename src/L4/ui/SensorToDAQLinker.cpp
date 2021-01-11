#include "SensorToDAQLinker.h"
#include "ui_SensorToDAQLinker.h"

SensorToDAQLinker::SensorToDAQLinker(QWidget *parent) : QDialog(parent), ui(new Ui::SensorToDAQLinker) {
    ui->setupUi(this);



    connect(ui->closeButton, &QPushButton::clicked, this, &SensorToDAQLinker::reject);
    connect(ui->linkButton, &QPushButton::clicked, this, &SensorToDAQLinker::accept);
}

SensorToDAQLinker::~SensorToDAQLinker() {
    delete ui;
}
