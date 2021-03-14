#include "DAQScanDialog.h"

DAQScanDialog::DAQScanDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::DAQScanPage)
{
    this->ui->setupUi(this);
    connect(this->ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(this->ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    this->displayDummyDAQs();
}

void
DAQScanDialog::displayDummyDAQs()
{
    // create device ID and insert into the map as false
    QString deviceID = "dummy:0";

    // create the checkbox and connect it
    QCheckBox* d = new QCheckBox(deviceID, this);
    d->setObjectName(deviceID);
    this->selectedDAQs.push_back(d);
    // create channels combo box here and register it
    QComboBox* cmb = new QComboBox(this);
    cmb->setObjectName(deviceID + "|numChannels");
    cmb->addItems({"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"});
    cmb->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // display on UI
    int row = this->ui->MCCDAQDevicesLayout->rowCount();
    this->ui->DummyDAQsLayout->addWidget(d, row, 0);
    this->ui->DummyDAQsLayout->addWidget(new QLabel("Number of channels: ", this), row, 1);
    this->ui->DummyDAQsLayout->addWidget(cmb, row, 2);
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
            LOG(INFO) << "Created DummyDAQ with " << deviceID.toStdString() << ", " << numChannels;
        }
//        else if (deviceID.contains("serialport")) {
//            std::string serialportPath = dqm->field(QString::fromStdString(deviceID + "|serialportPath")).toString().toStdString();
//            this->DAQDevices.push_back(new SerialPortDAQ(deviceID, numChannels, calibrationPoints, serialportPath));
//        }
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
