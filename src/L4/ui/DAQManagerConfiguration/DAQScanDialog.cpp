#include "DAQScanDialog.h"

DAQScanDialog::DAQScanDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::DAQScanDialog)
{
    this->ui->setupUi(this);
    connect(this->ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(this->ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    this->displayDummyDAQs();
    this->displayOpenSerialPorts();
}

void
DAQScanDialog::accept()
{
    for (const auto& d : this->selectedDAQs) {
        if (!d->isChecked()) { continue; }

        QString deviceID = d->objectName();
        if (deviceID.contains("dummy")) {
            unsigned int numChannels = this->findChild<QComboBox*>(deviceID + "|numChannels")->currentIndex() + 1;
            this->DAQDevices.push_back(new DummyDAQ(deviceID.toStdString(), numChannels));
        }
        else if (deviceID.contains("serialport")) {
            unsigned int numChannels = this->findChild<QComboBox*>(deviceID + "|numChannels")->currentIndex() + 1;
            std::string serialportPath = this->findChild<QLabel*>(deviceID + "|serialportPath")->text().toStdString();
            this->DAQDevices.push_back(new SerialPortDAQ(deviceID.toStdString(), numChannels, serialportPath));
        }
#ifdef ULDAQ_AVAILABLE
        else if (deviceID.contains("mccdaq")) {
            DaqDeviceHandle d = dqm->field(QString::fromStdString(deviceID + "|handle")).toLongLong();
            Range r = (Range)dqm->field(QString::fromStdString(deviceID + "|range")).toLongLong();
            this->DAQDevices.push_back(new AiMCCDAQ(deviceID, numChannels, calibrationPoints, d, r));
        }
#endif
#ifdef WIRINGPI_AVAILABLE
        else if (deviceID.find("i2c") != std::string::npos) {
            unsigned char addr = dqm->field(QString::fromStdString(deviceID + "|i2cAddr")).toUInt();
            DAQDevices.push_back(new I2CDAQ(deviceID, numChannels, calibrationPoints, addr));
        }
#endif
        else {
            LOG(FATAL) << "Internal error with DAQManagerWizard, abandon ship";
        }
    }

    this->done(QDialog::Accepted);
}

void
DAQScanDialog::displayDummyDAQs()
{
    // create device ID and checkbox and connect it
    QString deviceID = "dummy:0";
    QCheckBox* chb = new QCheckBox(deviceID, this);
    chb->setObjectName(deviceID);
    this->selectedDAQs.push_back(chb);
    // create channels combo box here and register it
    QComboBox* cmb = new QComboBox(this);
    cmb->setObjectName(deviceID + "|numChannels");
    cmb->addItems({"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"});
    cmb->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // display on UI
    int row = this->ui->MCCDAQDevicesLayout->rowCount();
    this->ui->DummyDAQsLayout->addWidget(chb, row, 0);
    this->ui->DummyDAQsLayout->addWidget(new QLabel("Number of channels: ", this), row, 1);
    this->ui->DummyDAQsLayout->addWidget(cmb, row, 2);
}

void
DAQScanDialog::displayOpenSerialPorts()
{
    // lambda to separate finding open ports from displaying them
    // kinda overkill but it's better than making a helper which is
    // used only once in my opinion.
    auto scanForOpenSerialPorts = []() -> std::vector<std::tuple<std::string, std::string>>
    {
        std::vector<std::tuple<std::string, std::string>> ports;
        QDir* d = new QDir("/dev","tty*", QDir::Name, QDir::System);
        for (const auto& f : d->entryList()) {
            std::string filename = f.toStdString();
            std::string path = "/dev/" + filename;
            std::ifstream test(path);
            if (test.is_open()) {
                ports.push_back(std::make_tuple(filename, path));
            }
        }
        return ports;
    };

    for (const auto& t : scanForOpenSerialPorts()) {
        // Create deviceID and checkbox
        QString deviceID = QString::fromStdString("serialport:" + std::get<0>(t));
        QCheckBox* chb = new QCheckBox(deviceID, this);
        chb->setObjectName(deviceID);
        this->selectedDAQs.push_back(chb);
        // Combo box for number of channels.
        QComboBox* cmb = new QComboBox(this);
        cmb->setObjectName(deviceID + "|numChannels");
        cmb->addItems({"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"});
        cmb->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        // Label for serial port file path
        QLabel* l = new QLabel(QString::fromStdString(std::get<1>(t)), this);
        l->setObjectName(deviceID + "|serialportPath");

        int row = this->ui->SerialportDevicesLayout->rowCount();
        this->ui->SerialportDevicesLayout->addWidget(chb, row, 0);
        this->ui->SerialportDevicesLayout->addWidget(new QLabel("Path: ", this), row, 1);
        this->ui->SerialportDevicesLayout->addWidget(l, row, 2);
        this->ui->SerialportDevicesLayout->addWidget(new QLabel("Number of 'channels': ", this), row, 3);
        this->ui->SerialportDevicesLayout->addWidget(cmb, row, 4);
    }
}
