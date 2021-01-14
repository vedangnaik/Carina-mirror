#include "DAQManagerFactory.h"

DAQManagerFactory::DAQManagerFactory(QWidget *parent) : QDialog(parent), ui(new Ui::DAQManagerFactory) {
    ui->setupUi(this);

    // connect MCCDAQ is available
#ifdef ULDAQ_AVAILABLE
    ui->MCCDAQGroupBox->setDisabled(false);
    connect(this->ui->MCCDAQScanButton, &QPushButton::clicked, this, &DAQManagerFactory::openAndTestAiMCCDAQs);
#else
    ui->MCCDAQDevicesLayout->addWidget(new QLabel("MCCDAQ support is not available on this platform.", this));
    ui->MCCDAQGroupBox->setDisabled(true);
#endif

    // connect serial ports
    QDir* d = new QDir("/dev","tty*", QDir::Name, QDir::System);
    ui->availableTTYsComboBox->addItems(d->entryList());
    connect(this->ui->serialportOpenButton, &QPushButton::clicked, this, &DAQManagerFactory::openAndTestSerialPort);

    // connect dialog button
    connect(this->ui->configureButton, &QPushButton::clicked, this, &DAQManagerFactory::accept);
    connect(this->ui->closeButton, &QPushButton::clicked, this, &DAQManagerFactory::reject);
}

#ifdef ULDAQ_AVAILABLE
void DAQManagerFactory::openAndTestAiMCCDAQs() {
    const DaqDeviceInterface DAQDeviceInterfaceType = ANY_IFC;

    // Get the number of connected devices here.
    std::vector<DaqDeviceDescriptor> devDescriptors;
    unsigned int numDAQDevicesDetected = 0;
    UlError err = ulGetDaqDeviceInventory(DAQDeviceInterfaceType, devDescriptors.data(), &numDAQDevicesDetected);
    // This will trip, but can be ignored safely (I think)
    if (err != ERR_NO_ERROR) {
        LOG(INFO) << "This 'error' is expected to occur, don't panic.";
        LOG(ERROR) << "ulGetDaqDeviceInventory Error: " << err;
    }

    // List DAQs found
    if (numDAQDevicesDetected == 0) {
        LOG(ERROR) << "No MCCDAQ devices found";
    }
    else {
        // Get the device descriptors here
        devDescriptors.reserve(numDAQDevicesDetected);
        UlError err = ulGetDaqDeviceInventory(DAQDeviceInterfaceType, devDescriptors.data(), &numDAQDevicesDetected);
        if (err != ERR_NO_ERROR) { LOG(ERROR) << "ulGetDaqDeviceInventory Error: " << err; }

        // Get DAQ info here
        for (unsigned int i = 0; i < numDAQDevicesDetected; i++) {
            // Create the DAQ object
            DaqDeviceHandle handle = ulCreateDaqDevice(devDescriptors[i]);

            // Check if it's analog input; other types can be checked for here
            long long aiSupported;
            UlError err = ulDevGetInfo(handle, DEV_INFO_HAS_AI_DEV, 0, &aiSupported);
            if (err != ERR_NO_ERROR) { LOG(ERROR) << "ulDevGetInfo Error: " << err; }

            // Get num channels
            long long numChannels;
            err = ulAIGetInfo(handle, AI_INFO_NUM_CHANS, 0, &numChannels);
            if (err != ERR_NO_ERROR) { /*shit */ std::cout << "ulAIGetInfo Error: " << err << std::endl; }
            if (aiSupported < 1) { LOG(ERROR) << "Analog input not supported: " << aiSupported; }

            // get voltage range
            long long voltageRange;
            err = ulAIGetInfo(handle, AI_INFO_SE_RANGE, 0, &voltageRange);
            if (err != ERR_NO_ERROR) { LOG(ERROR) << "ulAIGetInfo Error: " << err; }

            // output information here
            std::string deviceID = "mccdaq:" + std::to_string(i);
            std::string infoLine =
                    "Analog Input: " + std::string(aiSupported ? "yes" : "no") + "\t" +
                    "Number of channels: " + std::to_string(numChannels) + "\t" +
                    "Voltage range: " + std::to_string(voltageRange);

            int row = this->ui->MCCDAQDevicesLayout->rowCount();
            this->ui->MCCDAQDevicesLayout->addWidget(new QCheckBox(QString::fromStdString(deviceID), this), row, 0);
            this->ui->MCCDAQDevicesLayout->addWidget(new QLabel("Analog Input: " + QString(aiSupported ? "yes" : "no"), this), row, 1);
            this->ui->MCCDAQDevicesLayout->addWidget(new QLabel("Handle: ", this), row, 2);
            this->ui->MCCDAQDevicesLayout->addWidget(new QLabel(QString::number(handle), this), row, 3);
            this->ui->MCCDAQDevicesLayout->addWidget(new QLabel("Number of channels: ", this), row, 4);
            this->ui->MCCDAQDevicesLayout->addWidget(new QLabel(QString::number(numChannels), this), row, 5);
            this->ui->MCCDAQDevicesLayout->addWidget(new QLabel("Voltage range: ", this), row, 6);
            this->ui->MCCDAQDevicesLayout->addWidget(new QLabel(QString::number(voltageRange), this), row, 7);
        }
    }
}
#endif

void DAQManagerFactory::openAndTestSerialPort() {
    std::string serialportName = this->ui->availableTTYsComboBox->currentText().toStdString();
    std::ifstream test("/dev/" + serialportName);
    if (!test.is_open()) {
        LOG(ERROR) << "Could not open serial port: " << serialportName;
//        return;
    }
    this->ui->availableTTYsComboBox->removeItem(this->ui->availableTTYsComboBox->currentIndex());

    std::string deviceID = "serialport:" + serialportName;
    std::string infoLine =
            "Path: /dev/" + serialportName + "\t" +
            "Number of 'channels': ";

    QComboBox* cmb = new QComboBox(this);
    cmb->addItems({"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"});
    cmb->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    int row = this->ui->SerialportDevicesLayout->rowCount();
    this->ui->SerialportDevicesLayout->addWidget(new QCheckBox(QString::fromStdString(deviceID), this), row, 0);
    this->ui->SerialportDevicesLayout->addWidget(new QLabel("Path: ", this), row, 1);
    this->ui->SerialportDevicesLayout->addWidget(new QLabel(QString::fromStdString("/dev/" + serialportName), this), row, 2);
    this->ui->SerialportDevicesLayout->addWidget(new QLabel("\tNumber of 'channels': ", this), row, 3);
    this->ui->SerialportDevicesLayout->addWidget(cmb, row, 4);
}

DAQManagerFactory::~DAQManagerFactory() {
    delete ui;
}

std::unique_ptr<DAQManager> DAQManagerFactory::createDAQManager() {
    DAQManagerFactory dmf;
    int r = dmf.exec();
    if (r == QDialog::Accepted) {
        return std::make_unique<DAQManager>(dmf.prospectiveDAQDevices);
    } else {
        return nullptr;
    }
}

void DAQManagerFactory::accept() {
#ifdef ULDAQ_AVAILABLE
    // if available, loop through and create MCCDAQ handlers here
    for (int i = 1; i < this->ui->MCCDAQDevicesLayout->rowCount(); i++) {
        QCheckBox* chb = (QCheckBox*)this->ui->MCCDAQDevicesLayout->itemAtPosition(i, 0)->widget();
        if (chb->isChecked()) {
            QLabel* h = (QLabel*)this->ui->MCCDAQDevicesLayout->itemAtPosition(i, 3)->widget();
            QLabel* nc = (QLabel*)this->ui->MCCDAQDevicesLayout->itemAtPosition(i, 5)->widget();
            QLabel* vr = (QLabel*)this->ui->MCCDAQDevicesLayout->itemAtPosition(i, 7)->widget();
            this->prospectiveDAQDevices.push_back(
                new AiDAQHandler(chb->text().toStdString(), h->text().toLongLong(), nc->text().toUInt(), (Range)vr->text().toLongLong())
            );
        }
    }
#endif
    // loop through and create serial port handlers here
    for (int i = 1; i < this->ui->SerialportDevicesLayout->rowCount(); i++) {
        QCheckBox* chb = (QCheckBox*)this->ui->SerialportDevicesLayout->itemAtPosition(i, 0)->widget();
        if (chb->isChecked()) {
            QLabel* path = (QLabel*)this->ui->SerialportDevicesLayout->itemAtPosition(i, 2)->widget();
            QComboBox* cmb = (QComboBox*)this->ui->SerialportDevicesLayout->itemAtPosition(i, 4)->widget();
            this->prospectiveDAQDevices.push_back(
                new SerialPortHandler(chb->text().toStdString(), path->text().toStdString(), cmb->currentText().toUInt())
            );
        }
    }
    this->done(QDialog::Accepted);
}
