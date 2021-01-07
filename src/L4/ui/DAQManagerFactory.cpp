#include "DAQManagerFactory.h"

DAQManagerFactory::DAQManagerFactory(QWidget *parent) : QDialog(parent), ui(new Ui::DAQManagerFactory) {
    ui->setupUi(this);

    // connect MCCDAQ is available
#ifdef ULDAQ_AVAILABLE
    ui->MCCDAQGroupBox->setDisabled(false);
    connect(this->ui->MCCDAQScanButton, &QPushButton::clicked, this, &DAQManagerFactory::scanForAiMCCDAQs);
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
void DAQManagerFactory::scanForAiMCCDAQs() {
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
                    this->selectedAiMccdaqs.insert_or_assign(deviceID, aidi);
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
    std::ifstream test("/dev" + serialportName);
    if (!test.is_open()) {
        this->ui->serialportOpenButton->setStyleSheet("background-color: red");
        return;
    }
    this->ui->serialportOpenButton->setStyleSheet("background-color: green");

    std::string deviceID = "serialport:" + serialportName;
    std::string infoLine =
            "Path: /dev/" + serialportName + "\t" +
            "Number of 'channels': ";

    QWidget* w = new QWidget(this);
    QHBoxLayout* h = new QHBoxLayout(this);
    w->setLayout(h);
    QCheckBox* chb = new QCheckBox(QString::fromStdString(deviceID), this);
    QLabel* l = new QLabel(QString::fromStdString(infoLine), this);
    QSpinBox* b = new QSpinBox(this);
    b->setRange(1, 10);
    b->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    h->addWidget(chb);
    h->addWidget(l);
    h->addWidget(b);

    struct SerialPortInfo spi = { deviceID, "/dev/" + serialportName, (unsigned int)b->value() };

    connect(b, &QSpinBox::textChanged, this, [=](const QString& t) {
        if (this->selectedSerialports.find(deviceID) != this->selectedSerialports.end()) {
            this->selectedSerialports.at(deviceID).numChannels = t.toInt();
        }
    });
    connect(chb, &QCheckBox::stateChanged, this, [=](int state) {
        if (state == Qt::Checked) {
            this->selectedSerialports.insert_or_assign(deviceID, spi);
        } else if (state == Qt::Unchecked) {
            this->selectedSerialports.erase(deviceID);
        }
    });

    this->ui->SerialportDevicesLayout->addWidget(w);
}

DAQManagerFactory::~DAQManagerFactory() {
    delete ui;
}

std::unique_ptr<DAQManager> DAQManagerFactory::createDAQManager() {
    DAQManagerFactory dmf;
    int r = dmf.exec();
    if (r == QDialog::Accepted) {
        std::vector<IDAQDeviceHandler*> DAQDevices;
#ifdef ULDAQ_AVAILABLE
        for (const auto& [id, devInfo]: dmf.selectedAiMccdaqs) {
            DAQDevices.push_back(
                new AiDAQHandler(devInfo.id, devInfo.handle, devInfo.numChannels, devInfo.voltageRange)
            );
        }
#endif
        for (const auto& [id, devInfo] : dmf.selectedSerialports) {
            DAQDevices.push_back(
                new SerialPortHandler(devInfo.id, devInfo.serialportPath, devInfo.numChannels)
            );
        }
        return std::make_unique<DAQManager>(DAQDevices);
    }
    return nullptr;
}
