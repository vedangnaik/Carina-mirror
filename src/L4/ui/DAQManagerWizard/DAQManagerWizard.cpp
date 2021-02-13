#include "DAQManagerWizard.h"

DAQManagerWizard::DAQManagerWizard(std::unique_ptr<DAQManager> daqm, DAQManagerWizardAction a, std::vector<std::string> sensorIDs, QWidget* parent)
    : QWizard(parent), daqm{std::move(daqm)}
{
    // cheeky trick to exploit a side-effect/possible bug of QDialog where the internal state
    // is saved on accept, but not on reject for some strange reason.
    this->setOption(QWizard::NoCancelButton);
    this->setOption(QWizard::HaveCustomButton1);
    this->setButtonText(QWizard::CustomButton1, "Cancel");
    connect(this->button(QWizard::CustomButton1), &QAbstractButton::clicked, this, &QDialog::accept);
    // rearrange the buttons to look nicer.
    QList<QWizard::WizardButton> layout;
    layout << QWizard::CustomButton1 << QWizard::Stretch<< QWizard::BackButton << QWizard::NextButton << QWizard::FinishButton;
    this->setButtonLayout(layout);


    switch(a) {
    case Manufacture: {
        this->setWindowTitle("DAQ Manager Configuration Wizard");
        this->addPage(new DAQScanPage());
        this->addPage(new DAQCalibrationPage());
        this->addPage(new DAQLinkingPage(sensorIDs));
        break;
    }
    case Recalibrate: {
        this->setWindowTitle("DAQ Manager Re-calibration Wizard");
        auto dcp = new DAQCalibrationPage();
        for (const auto& daq : this->daqm->DAQDevices) {
            this->abstractDAQData.insert({ daq->deviceID, true });
            dcp->calibrationPoints.insert({ daq->deviceID, daq->calibrationPoints });
        }
        this->addPage(dcp);
        break;
    }
    case Relink: {
        this->setWindowTitle("DAQ Manager Sensor <-> Channel Re-link Wizard");
        auto dlp = new DAQLinkingPage(sensorIDs);
        for (const auto& p : this->daqm->sensorToDAQLinks) {
            const std::string& sensorID = p.first;
            const std::string& deviceID = p.second.first->deviceID;
            const unsigned int channel = p.second.second;
            dlp->sensorLinks.insert({ sensorID, deviceID + "-" + std::to_string(channel) });
            this->abstractDAQData.insert({ deviceID, true });
        }
        this->addPage(dlp);
        break;
    }
    }
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

//void
//DAQManagerWizard::recalibrateDAQs()
//{
//    auto* dcp = (DAQCalibrationPage*)this->page(0);
//    for (const auto& p : dcp->calibrationPoints) {
//        AbstractDAQ* daq = *std::find_if(this->daqm->DAQDevices.begin(), this->daqm->DAQDevices.end(), [=](AbstractDAQ* daq) {
//            return daq->deviceID == p.first;
//        });
//        daq->calibrate(p.second);
//    }
//}

//void
//DAQManagerWizard::relinkSensors()
//{
//    auto* dlp = (DAQLinkingPage*)this->page(0);
//    this->daqm->sensorToDAQLinks.clear();
//    for (const auto& p : dlp->sensorLinks) {
//        if (p.second == "<unlinked>") continue;

//        const auto& IDAndChannel = p.second;
//        std::string::size_type n = IDAndChannel.find("-");
//        std::string id = IDAndChannel.substr(0, n);
//        unsigned int channel = std::stoul(IDAndChannel.substr(n+1, IDAndChannel.length()));

//        AbstractDAQ* daq = *std::find_if(this->daqm->DAQDevices.begin(), this->daqm->DAQDevices.end(), [=](AbstractDAQ* d) {
//            return d->deviceID == id;
//        });
//        this->daqm->sensorToDAQLinks.insert({ p.first, std::make_pair(daq, channel) });
//    }
//}

std::unique_ptr<DAQManager>
DAQManagerWizard::getDAQManager() {
    return std::move(this->daqm);
}



std::unique_ptr<DAQManager>
DAQManagerWizard::manufactureDAQManager(std::vector<std::string> sensorIDs)
{
    DAQManagerWizard dmw(std::move(daqm), DAQManagerWizardAction::Manufacture, sensorIDs);
    LOG(INFO) << dmw.exec();
    if (dmw.exec() == QDialog::Accepted) {
        dmw.manufactureDAQManager();
        return dmw.getDAQManager();
    } else {
        return nullptr;
    }
}

//std::unique_ptr<DAQManager>
//DAQManagerWizard::recalibrateDAQs(std::unique_ptr<DAQManager> daqm)
//{
//    DAQManagerWizard dmw(std::move(daqm), DAQManagerWizardAction::Recalibrate);
//    if (dmw.exec() == QDialog::Accepted) {
//        dmw.recalibrateDAQs();
//    }
//    return dmw.getDAQManager();
//}

//std::unique_ptr<DAQManager>
//DAQManagerWizard::relinkSensors(std::unique_ptr<DAQManager> daqm, std::vector<std::string> sensorIDs)
//{
//    DAQManagerWizard dmw(std::move(daqm), DAQManagerWizardAction::Relink, sensorIDs);
//    if (dmw.exec() == QDialog::Accepted) {
//        dmw.relinkSensors();
//    }
//    return dmw.getDAQManager();
//}
