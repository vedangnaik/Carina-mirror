#include "DAQManagerWizard.h"

DAQManagerWizard::DAQManagerWizard(QWidget* parent)
    : QWizard(parent)
{
    this->addPage(new DAQScanPage());
    this->addPage(new DAQCalibrationPage());
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
