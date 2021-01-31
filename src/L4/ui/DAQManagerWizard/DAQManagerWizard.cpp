#include "DAQManagerWizard.h"

DAQManagerWizard::DAQManagerWizard(std::vector<std::string> sensorIDs, QWidget* parent)
    : QWizard(parent)
{
    this->setWindowTitle("DAQ Manager Configuration Wizard");
    this->addPage(new DAQScanPage());
    this->addPage(new DAQCalibrationPage());
    this->addPage(new DAQLinkingPage(sensorIDs));
}

DAQManagerWizard::DAQManagerWizard(std::unique_ptr<DAQManager> daqm, QWidget* parent)
    : QWizard(parent), daqm{std::move(daqm)}
{
    this->setWindowTitle("DAQ Manager Re-calibration Wizard");
    auto dcp = new DAQCalibrationPage();
    for (const auto& daq : this->daqm->DAQDevices) {
        this->abstractDAQData.insert({ daq->deviceID, true });
        dcp->calibrationPoints.insert({ daq->deviceID, daq->calibrationPoints });
    }
    this->addPage(dcp);
}

void DAQManagerWizard::manufactureDAQManager()
{
    auto* dcp = (DAQCalibrationPage*)this->page(1);
    auto* dlp = (DAQLinkingPage*)this->page(2);

    // assemble vector of abstract daq devices
    std::vector<AbstractDAQ*> DAQDevices;
    for (const auto& p : this->abstractDAQData) {
        if (!p.second) continue;

        // Ugly searching based on substring xD
        auto& deviceID = p.first;
        unsigned int numChannels = this->field(QString::fromStdString(deviceID + "|numChannels")).toUInt() + 1;
        const auto& calibrationPoints = dcp->calibrationPoints.at(deviceID);
        // Dummy DAQ Assembly
        if (deviceID.find("dummy") != std::string::npos) {
            DAQDevices.push_back(new DummyDAQ(deviceID, numChannels, calibrationPoints));
        } else if (deviceID.find("serialport") != std::string::npos) {
            std::string serialportPath = this->field(QString::fromStdString(deviceID + "|serialportPath")).toString().toStdString();
            DAQDevices.push_back(new SerialPortDAQ(deviceID, numChannels, calibrationPoints, serialportPath));
#ifdef ULDAQ_AVAILABLE
        } else if (deviceID.find("mccdaq") != std::string::npos) {
            DaqDeviceHandle d = this->field(QString::fromStdString(deviceID + "|handle")).toLongLong();
            Range r = (Range)this->field(QString::fromStdString(deviceID + "|range")).toLongLong();
            DAQDevices.push_back(new AiMCCDAQ(deviceID, numChannels, calibrationPoints, d, r));
#endif
        } else {
            LOG(FATAL) << "Internal error with DAQManagerWizard, abandon ship";
        }
    }

    // assemble map of links
    std::map<std::string, std::pair<AbstractDAQ*, unsigned int>> sensorToDAQLinks;
    for (const auto& p : dlp->sensorLinks) {
        if (p.second == "<unlinked>") continue;

        const auto& IDAndChannel = p.second;
        std::string::size_type n = IDAndChannel.find("-");
        std::string id = IDAndChannel.substr(0, n);
        unsigned int channel = std::stoul(IDAndChannel.substr(n+1, IDAndChannel.length()));

        AbstractDAQ* daq = *std::find_if(DAQDevices.begin(), DAQDevices.end(), [=](AbstractDAQ* d) {
            return d->deviceID == id;
        });
        sensorToDAQLinks.insert({ p.first, std::make_pair(daq, channel) });
    }

    this->daqm = std::make_unique<DAQManager>(DAQDevices, sensorToDAQLinks);
}

void
DAQManagerWizard::recalibrateDAQs()
{
    auto* dcp = (DAQCalibrationPage*)this->page(0);
    for (const auto& p : dcp->calibrationPoints) {
        AbstractDAQ* daq = *std::find_if(this->daqm->DAQDevices.begin(), this->daqm->DAQDevices.end(), [=](AbstractDAQ* daq) {
            return daq->deviceID == p.first;
        });
        daq->calibrate(p.second);
    }
}

std::unique_ptr<DAQManager>
DAQManagerWizard::manufactureDAQManager(std::vector<std::string> sensorIDs)
{
    DAQManagerWizard dmw(sensorIDs);
    if (dmw.exec() == QDialog::Accepted) {
        dmw.manufactureDAQManager();
        return std::move(dmw.daqm);
    } else {
        return nullptr;
    }
}

std::unique_ptr<DAQManager>
DAQManagerWizard::recalibrateDAQs(std::unique_ptr<DAQManager> daqm)
{
    DAQManagerWizard dmw(std::move(daqm));
    if (dmw.exec() == QDialog::Accepted) {
        dmw.recalibrateDAQs();
    }
    return std::move(dmw.daqm);
}
