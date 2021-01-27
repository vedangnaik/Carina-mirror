#include "DAQManagerWizard.h"

DAQManagerWizard::DAQManagerWizard(std::vector<std::string> sensorIDs, QWidget* parent)
    : QWizard(parent)
{
    this->addPage(new DAQScanPage());
    this->addPage(new DAQCalibrationPage());
    this->addPage(new DAQLinkingPage(sensorIDs));
}

void
DAQManagerWizard::accept()
{
    QDialog::accept();
}

void
DAQManagerWizard::reject()
{
    QDialog::reject();
}
