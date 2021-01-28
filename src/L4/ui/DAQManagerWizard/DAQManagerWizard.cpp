#include "DAQManagerWizard.h"

DAQManagerWizard::DAQManagerWizard(std::vector<std::string> sensorIDs, SVGIC& svgic, QWidget* parent)
    : QWizard(parent), svgic{svgic}
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
            DAQDevices.push_back(new DummyDAQ(id, numChannels, calibrationPoints));
        }
    }

    // assemble map of links
    std::map<std::string, std::pair<AbstractDAQ*, unsigned int>> sensorToDAQLinks;
    for (const auto& p : dlp->sensorLinks) {
        if (p.second == "<unlinked>") continue;

        const auto& IDAndChannel = p.second;
        std::string::size_type n = IDAndChannel.find("-");
        std::string id = IDAndChannel.substr(0, n);
        unsigned int numChannels = std::stoul(IDAndChannel.substr(n, IDAndChannel.length()));

        AbstractDAQ* daq = *std::find_if(DAQDevices.begin(), DAQDevices.end(), [=](AbstractDAQ* d) {
            return d->deviceID == id;
        });
        sensorToDAQLinks.insert({ p.first, std::make_pair(daq, numChannels) });
    }

    this->daqm = std::make_unique<DAQManager>(DAQDevices, sensorToDAQLinks, this->svgic);
    QDialog::done(QDialog::Accepted);
}

void
DAQManagerWizard::reject()
{
    QDialog::done(QDialog::Rejected);
}

std::unique_ptr<DAQManager>
DAQManagerWizard::setupDAQManager(std::vector<std::string> sensorIDs, SVGIC& svgic)
{
    std::unique_ptr<DAQManager> daqm;
    DAQManagerWizard dmw(sensorIDs, svgic);
    if (dmw.exec() == QDialog::Accepted) {
        daqm = std::move(dmw.daqm);
        return daqm;
    } else {
        return nullptr;
    }
}
