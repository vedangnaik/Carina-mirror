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
        QLabel* l = new QLabel("No MCCDAQ devices were found.", this);
        ui->MCCDAQDevicesLayout->addWidget(l);
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

            QWidget* w = new QWidget(this);
            QHBoxLayout* h = new QHBoxLayout(this);
            w->setLayout(h);
            QCheckBox* chb = new QCheckBox(QString::fromStdString(deviceID), this);
            QLabel* l = new QLabel(QString::fromStdString(infoLine), this);
            l->setWordWrap(true);
            h->addWidget(chb);
            h->addWidget(l);

            struct AiDAQInfo aidi = { deviceID, handle, (unsigned int)numChannels, (Range)voltageRange };
            connect(chb, &QCheckBox::stateChanged, this, [=](int state) {
                if (state == Qt::Checked) {
                    this->selectedAiMccdaqs.insert({deviceID, aidi});
                } else if (state == Qt::Unchecked) {
                    this->selectedAiMccdaqs.erase(deviceID);
                }
            });

            ui->MCCDAQDevicesLayout->addWidget(w);
        }
    }
}
#endif

void DAQManagerFactory::openAndTestSerialPort() {
    std::string serialportName = ui->availableTTYsComboBox->currentText().toStdString();
    std::ifstream test("/dev/" + serialportName);
    if (!test.is_open()) {
        LOG(ERROR) << "Could not open serial port: " << serialportName;
//        return;
    }
    // remove entry from combo box here.

    std::string deviceID = "serialport:" + serialportName;
    std::string infoLine =
            "Path: /dev/" + serialportName + "\t" +
            "Number of 'channels': ";

    QCheckBox* chb = new QCheckBox(QString::fromStdString(deviceID), this);
    QLabel* pl = new QLabel("Path: ", this);
    QLabel* path = new QLabel(QString::fromStdString("/dev/" + serialportName), this);
    QLabel* cl = new QLabel("\tNumber of 'channels': ", this);
    QComboBox* cmb = new QComboBox(this);
    cmb->addItems({"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"});
    cmb->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    int row = this->ui->SerialportDevicesLayout->rowCount();
    this->ui->SerialportDevicesLayout->addWidget(chb, row, 0);
    this->ui->SerialportDevicesLayout->addWidget(pl, row, 1);
    this->ui->SerialportDevicesLayout->addWidget(path, row, 2);
    this->ui->SerialportDevicesLayout->addWidget(cl, row, 3);
    this->ui->SerialportDevicesLayout->addWidget(cmb, row, 4);

    struct SerialPortInfo spi = { deviceID, "/dev/" + serialportName, static_cast<unsigned int>(cmb->currentIndex() + 1) };

//    connect(cmb, &QComboBox::currentTextChanged, this, [=](const QString& t) {
//        this->selectedSerialports.at(deviceID).numChannels = std::stoi(t.toStdString());
//    });
//    connect(chb, &QCheckBox::stateChanged, this, [=](int state) {
//        if (state == Qt::Checked) {
//            this->selectedSerialports.insert({deviceID, spi});
//            cmb->setEnabled(true);
//        } else if (state == Qt::Unchecked) {
//            this->selectedSerialports.erase(deviceID);
//            cmb->setEnabled(false);
//            this->selectedSerialports.at(deviceID).numChannels = std::stoi(cmb->currentText().toStdString());
//        }
//    });
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
    for (int i = 0; i < this->ui->MCCDAQDevicesLayout->count(); i++) {

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

void DAQManagerFactory::reject() {
    this->done(QDialog::Rejected);
}
