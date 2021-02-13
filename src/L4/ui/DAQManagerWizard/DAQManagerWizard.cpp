#include "DAQManagerWizard.h"

DAQManagerWizard::DAQManagerWizard(std::vector<std::string> sensorIDs, QWidget* parent)
    : QWizard(parent)
{
    // disable close button so that user can't exit that way.
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint);

    // cheeky trick to exploit a side-effect/possible bug of QDialog where the internal
    // state is saved on accept, but not on reject for some strange reason.
    this->setOption(QWizard::NoCancelButton);
    this->setOption(QWizard::HaveCustomButton1);
    this->setButtonText(QWizard::CustomButton1, "Cancel");
    connect(this->button(QWizard::CustomButton1), &QAbstractButton::clicked, this, &DAQManagerWizard::reject);

    // connect finish to custom accept
    connect(this->button(QWizard::FinishButton), &QAbstractButton::clicked, this, &DAQManagerWizard::accept);

    // rearrange the buttons to look nicer.
    QList<QWizard::WizardButton> layout;
    layout << QWizard::CustomButton1 << QWizard::Stretch<< QWizard::BackButton << QWizard::NextButton << QWizard::FinishButton;
    this->setButtonLayout(layout);

    // Make manufacturing layout
    this->setWindowTitle("DAQ Manager Configuration/Re-configuation Wizard");
    this->addPage(new DAQScanPage());
    this->addPage(new DAQCalibrationPage());
    this->addPage(new DAQLinkingPage(sensorIDs));
}

void
DAQManagerWizard::accept()
{
    this->done = true;
    QDialog::accept();
}

void
DAQManagerWizard::reject()
{
    this->done = false;
    QDialog::accept();
}

// initialize the static variable here
DAQManagerWizard* DAQManagerWizard::dqm = nullptr;

std::unique_ptr<DAQManager>
DAQManagerWizard::manufactureDAQManager(std::vector<std::string> sensorIDs)
{
    // for manufacturing, create a new wizard.
    dqm = new DAQManagerWizard(sensorIDs);
    dqm->exec();
    return dqm->done ? DAQManagerWizard::assembleDAQManager() : nullptr;
}

std::unique_ptr<DAQManager>
DAQManagerWizard::reconfigureDAQManager()
{
    // for reconfiguring, try to use the old one if possible.
    if (dqm == nullptr) {
        LOG(ERROR) << "No DAQManager to reconfigure. Please create a new DAQManager first.";
        return nullptr;
    } else {
        dqm->exec();
        return dqm->done ? DAQManagerWizard::assembleDAQManager() : nullptr;
    }
}

std::unique_ptr<DAQManager>
DAQManagerWizard::assembleDAQManager()
{
    auto* dcp = (DAQCalibrationPage*)dqm->page(1);
    auto* dlp = (DAQLinkingPage*)dqm->page(2);

    // assemble vector of abstract daq devices
    std::vector<AbstractDAQ*> DAQDevices;
    for (const auto& p : dqm->abstractDAQData) {
        if (!p.second) continue;

        // Ugly searching based on substring xD
        auto& deviceID = p.first;
        unsigned int numChannels = dqm->field(QString::fromStdString(deviceID + "|numChannels")).toUInt() + 1;
        const auto& calibrationPoints = dcp->calibrationPoints.at(deviceID);
        // Dummy DAQ Assembly
        if (deviceID.find("dummy") != std::string::npos) {
            DAQDevices.push_back(new DummyDAQ(deviceID, numChannels, calibrationPoints));
        } else if (deviceID.find("serialport") != std::string::npos) {
            std::string serialportPath = dqm->field(QString::fromStdString(deviceID + "|serialportPath")).toString().toStdString();
            DAQDevices.push_back(new SerialPortDAQ(deviceID, numChannels, calibrationPoints, serialportPath));
#ifdef ULDAQ_AVAILABLE
        } else if (deviceID.find("mccdaq") != std::string::npos) {
            DaqDeviceHandle d = dqm->field(QString::fromStdString(deviceID + "|handle")).toLongLong();
            Range r = (Range)dqm->field(QString::fromStdString(deviceID + "|range")).toLongLong();
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

    return std::make_unique<DAQManager>(DAQDevices, sensorToDAQLinks);
}
