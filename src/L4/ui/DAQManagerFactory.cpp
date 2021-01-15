#include "DAQManagerFactory.h"


// Forward declared helper functions.
#ifdef ULDAQ_AVAILABLE
std::vector<std::tuple<DaqDeviceHandle, bool, unsigned int, Range>> scanForAiMCCDAQs();
#endif
std::vector<std::tuple<std::string, std::string>> scanForOpenSerialPorts();


DAQManagerFactory::DAQManagerFactory(QWidget *parent) : QDialog(parent), ui(new Ui::DAQManagerFactory)
{
    ui->setupUi(this);

#ifdef ULDAQ_AVAILABLE
    // connect MCCDAQ is available
    for (const auto& t : scanForAiMCCDAQs()) {
        std::string deviceID = &"mccdaq:" [ std::get<0>(t)];
        int row = this->ui->MCCDAQDevicesLayout->rowCount();
        this->ui->MCCDAQDevicesLayout->addWidget(new QCheckBox(QString::fromStdString(deviceID), this), row, 0);
        this->ui->MCCDAQDevicesLayout->addWidget(new QLabel("Analog Input: " + QString(std::get<1>(t) ? "yes" : "no"), this), row, 1);
        this->ui->MCCDAQDevicesLayout->addWidget(new QLabel("Handle: ", this), row, 2);
        this->ui->MCCDAQDevicesLayout->addWidget(new QLabel(QString::number(std::get<0>(t)), this), row, 3);
        this->ui->MCCDAQDevicesLayout->addWidget(new QLabel("Number of channels: ", this), row, 4);
        this->ui->MCCDAQDevicesLayout->addWidget(new QLabel(QString::number(std::get<2>(t)), this), row, 5);
        this->ui->MCCDAQDevicesLayout->addWidget(new QLabel("Voltage range: ", this), row, 6);
        this->ui->MCCDAQDevicesLayout->addWidget(new QLabel(QString::number(std::get<3>(t)), this), row, 7);
    }
#else
    ui->MCCDAQDevicesLayout->addWidget(new QLabel("MCCDAQ support is not available on this platform.", this));
    ui->MCCDAQGroupBox->setDisabled(true);
#endif

    // connect serial ports
    for (const auto& t : scanForOpenSerialPorts()) {
        std::string deviceID = "serialport:" + std::get<0>(t);
        QComboBox* cmb = new QComboBox(this);
        cmb->addItems({"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"});
        cmb->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

        int row = this->ui->SerialportDevicesLayout->rowCount();
        this->ui->SerialportDevicesLayout->addWidget(new QCheckBox(QString::fromStdString(deviceID), this), row, 0);
        this->ui->SerialportDevicesLayout->addWidget(new QLabel("Path: ", this), row, 1);
        this->ui->SerialportDevicesLayout->addWidget(new QLabel(QString::fromStdString(std::get<1>(t)), this), row, 2);
        this->ui->SerialportDevicesLayout->addWidget(new QLabel("\tNumber of 'channels': ", this), row, 3);
        this->ui->SerialportDevicesLayout->addWidget(cmb, row, 4);
    }

    // connect dialog button
    connect(this->ui->configureButton, &QPushButton::clicked, this, &DAQManagerFactory::accept);
    connect(this->ui->closeButton, &QPushButton::clicked, this, &DAQManagerFactory::reject);
}

DAQManagerFactory::~DAQManagerFactory() {
    delete ui;
}

void DAQManagerFactory::accept() {
    // Note: QGridLayout row numberings start from 1 for some reason.
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

std::unique_ptr<DAQManager> DAQManagerFactory::createDAQManager() {
    DAQManagerFactory dmf;
    if (dmf.exec() == QDialog::Accepted) {
        return std::make_unique<DAQManager>(dmf.prospectiveDAQDevices);
    } else {
        return nullptr;
    }
}

#ifdef ULDAQ_AVAILABLE
std::vector<std::tuple<DaqDeviceHandle, bool, unsigned int, Range>> scanForAiMCCDAQs()
{
    std::vector<std::tuple<DaqDeviceHandle, bool, unsigned int, Range>> daqs;

    // Interface type to look for.
    const DaqDeviceInterface DAQDeviceInterfaceType = ANY_IFC;
    // Common error variable.
    UlError err;
    // Get the number of connected devices here.
    std::vector<DaqDeviceDescriptor> devDescriptors;
    unsigned int numDAQDevicesDetected = 0;
    err = ulGetDaqDeviceInventory(DAQDeviceInterfaceType, devDescriptors.data(), &numDAQDevicesDetected);
    // This will trip, but can be ignored safely (I think)
    if (err != ERR_NO_ERROR) { LOG(INFO) << "Expected 'error': ulGetDaqDeviceInventory Error: " << err; }

    // No DAQS are found, return empty vector.
    if (numDAQDevicesDetected == 0) {
        LOG(ERROR) << "No MCCDAQ devices found";

    // Populate vector of DAQS if any are found.
    } else {
        // Populate the vector of descriptors by calling this function again with the right number of connected daqs.
        devDescriptors.reserve(numDAQDevicesDetected);
        err = ulGetDaqDeviceInventory(DAQDeviceInterfaceType, devDescriptors.data(), &numDAQDevicesDetected);
        if (err != ERR_NO_ERROR) { LOG(ERROR) << "ulGetDaqDeviceInventory Error: " << err; }

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

            daqs.push_back(std::make_tuple(handle, bool(aiSupported), (unsigned int)numChannels, (Range)voltageRange));
        }
    }
    return daqs;
}
#endif

std::vector<std::tuple<std::string, std::string>> scanForOpenSerialPorts()
{
    std::vector<std::tuple<std::string, std::string>> ports;
    QDir* d = new QDir("/dev","tty*", QDir::Name, QDir::System);
    for (const auto& f : d->entryList()) {
        std::string filename = f.toStdString();
        std::string path = "/dev/" + filename;
        std::ifstream test(path);
//        if (test.is_open()) {
            ports.push_back(std::make_tuple(filename, path));
//        }
    }
    return ports;
}
