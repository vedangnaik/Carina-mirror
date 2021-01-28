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
    auto* dcp = (DAQCalibrationPage*)this->page(1);
    auto* dlp = (DAQLinkingPage*)this->page(2);

    // assemble vector of abstract daq devices
    std::vector<AbstractDAQ*> DAQDevices;
    for (const auto& p : this->abstractDAQData) {
        if (!p.second) continue;

        // Ugly searching based on substring xD
        auto& id = p.first;
        // Dummy DAQ Assembly
        if (id.find("dummy") != std::string::npos) {
            unsigned int numChannels = this->field(QString::fromStdString(id + "|numChannels")).toUInt();
            const auto& calibrationPoints = dcp->calibrationPoints.at(id);
            //DAQDevices.push_back(new DummyDAQ(id, numChannels, calibrationPoints));
        }
    }

    // assemble map of links
    for (const auto& p : dlp->sensorLinks) {
        LOG(INFO) << p.first << ": " << p.second;
    }


    QDialog::done(QDialog::Accepted);
}

void
DAQManagerWizard::reject()
{
    QDialog::done(QDialog::Rejected);
}
