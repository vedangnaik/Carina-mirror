#include "DAQManagerFactory.h"
#include "ui_daqmanagerfactory.h"

DAQManagerFactory::DAQManagerFactory(QWidget *parent) : QWidget(parent), ui(new Ui::DAQManagerFactory) {
    ui->setupUi(this);
}

std::unique_ptr<DAQManager> DAQManagerFactory::getConfiguredDAQManager() {

}

DAQManagerFactory::~DAQManagerFactory() {
    delete ui;
}
