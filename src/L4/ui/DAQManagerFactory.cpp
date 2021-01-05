#include "DAQManagerFactory.h"

DAQManagerFactory::DAQManagerFactory(QWidget *parent) : QWidget(parent), ui(new Ui::DAQManagerFactory) {
    ui->setupUi(this);
    // connect MCCDAQ is available
#ifdef ULDAQ_AVAILABLE
    ui->MCCDAQGroupBox->setDisabled(false);
    connect(this->ui->MCCDAQScanButton, &QPushButton::clicked, this, &DAQManagerFactory::scanForMCCDAQs);
#else
    ui->MCCDAQGroupBox->setDisabled(true);
#endif
    // connect serial ports
    QDir* d = new QDir("/dev","tty*", QDir::Name, QDir::System);
    ui->availableTTYsComboBox->addItems(d->entryList());
    connect(this->ui->serialportOpenButton, &QPushButton::clicked, this, &DAQManagerFactory::openAndTestSerialPort);
}

#ifdef ULDAQ_AVAILABLE
void DAQManagerFactory::scanForMCCDAQs() {
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
            std::string deviceID = "MCCDAQDevice" + std::to_string(i);
            std::string infoLine =
                    "ID: " + deviceID + "\n" +
                    "Analog Input: " + (aiSupported ? "yes" : "no") + "\n" +
                    "Number of channels: " + std::to_string(numChannels) + "\n" +
                    "Voltage range: " + std::to_string(voltageRange);
            QLabel* l = new QLabel(QString::fromStdString(infoLine), this);
            ui->MCCDAQDevicesLayout->addWidget(l);
        }
    }
}
#endif

void DAQManagerFactory::openAndTestSerialPort() {
    std::string serialportPath = "/dev/" + ui->availableTTYsComboBox->currentText().toStdString();
    std::ifstream test(serialportPath);
    if (!test.is_open()) {
        this->ui->serialportOpenButton->setStyleSheet("background-color: red");
        return;
    }
    this->ui->serialportOpenButton->setStyleSheet("background-color: green");

    QWidget* w = new QWidget(this);
    QHBoxLayout* h = new QHBoxLayout(this);
    w->setLayout(h);
    QLabel* l = new QLabel(QString::fromStdString("Serial Port path: " + serialportPath), this);
    l->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    QLabel* r = new QLabel("Number of 'channels'", this);
    QSpinBox* b = this->getSerialPortChannelsSpinBox();
    h->addWidget(l);
    h->addWidget(r);
    h->addWidget(b);

    this->ui->SerialportDevicesLayout->addWidget(w);
}

DAQManagerFactory::~DAQManagerFactory() {
    delete ui;
}

QSpinBox* DAQManagerFactory::getSerialPortChannelsSpinBox() {
    QSpinBox* b = new QSpinBox(this);
    b->setRange(1, 10);
    return b;
}
