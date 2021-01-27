#include "DAQManagerWizard.h"

DAQManagerWizard::DAQManagerWizard(std::vector<std::string> sensorIDs, QWidget* parent)
    : QWizard(parent)
{
    this->setWindowTitle("DAQ Manager Configuration Wizard");
    this->addPage(new DAQScanPage());
    this->addPage(new DAQCalibrationPage());
    this->addPage(new DAQLinkingPage(sensorIDs));
}

void
DAQManagerWizard::accept()
{
    // assemble vector of abstract daq devices
    // assemble vectors of calibration data for same
    // assemble map of links
    QDialog::done(QDialog::Accepted);
}

void
DAQManagerWizard::reject()
{
    QDialog::done(QDialog::Rejected);
}
