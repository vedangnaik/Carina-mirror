#include "DAQScanDialog.h"

DAQScanDialog::DAQScanDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::DAQScanDialog)
{
    this->ui->setupUi(this);

    this->displayDummyDAQs();
    this->displayOpenSerialPorts();
#ifdef ULDAQ_AVAILABLE
    this->displayAvailableAiMCCDAQs();
#endif
#ifdef WIRINGPI_AVAILABLE
    this->displayAvailableI2CDAQs();
#endif
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
            unsigned int numChannels = this->findChild<QLabel*>(deviceID + "|numChannels")->text().toUInt();
            DaqDeviceHandle handle = this->findChild<QLabel*>(deviceID + "|handle")->text().toLongLong();
            Range range = (Range)this->findChild<QLabel*>(deviceID + "|range")->text().toLongLong();
            this->DAQDevices.push_back(new AiMCCDAQ(deviceID.toStdString(), numChannels, handle, range));
        }
#endif
#ifdef WIRINGPI_AVAILABLE
        else if (deviceID.contains("i2c")) {
            unsigned int numChannels = this->findChild<QComboBox*>(deviceID + "|numChannels")->currentIndex() + 1;
            unsigned char addr = this->findChild<QLabel*>(deviceID + "|addr")->text().toUInt();
            DAQDevices.push_back(new I2CDAQ(deviceID.toStdString(), numChannels, addr));
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
        QDir d("/dev","tty*", QDir::Name, QDir::System);
        for (const auto& f : d.entryList()) {
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

#ifdef ULDAQ_AVAILABLE
void
DAQScanDialog::displayAvailableAiMCCDAQs()
{
    auto scanForAvailableAiMCCDAQs = []() -> std::vector<std::tuple<DaqDeviceHandle, bool, unsigned int, Range>>
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
        if (numDAQDevicesDetected == 0) return daqs;

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
            err = ulAIGetInfo(handle, AI_INFO_NUM_CHANS_BY_MODE, AI_SINGLE_ENDED, &numChannels);
            if (err != ERR_NO_ERROR) { /*shit */ std::cout << "ulAIGetInfo Error: " << err << std::endl; }
            if (aiSupported < 1) { LOG(ERROR) << "Analog input not supported: " << aiSupported; }

            // get voltage range
            long long voltageRange;
            err = ulAIGetInfo(handle, AI_INFO_SE_RANGE, 0, &voltageRange);
            if (err != ERR_NO_ERROR) { LOG(ERROR) << "ulAIGetInfo Error: " << err; }


            daqs.push_back(std::make_tuple(handle, bool(aiSupported), (unsigned int)numChannels, (Range)voltageRange));
        }

        return daqs;
    };

    for (const auto& t : scanForAvailableAiMCCDAQs()) {
        // create deviceID and checkbox
        QString deviceID = QString::fromStdString("mccdaq:" + std::to_string(std::get<0>(t)));
        QCheckBox* chb = new QCheckBox(deviceID, this);
        chb->setObjectName(deviceID);
        this->selectedDAQs.push_back(chb);
        // Labels for num channels, voltage range, daq handle
        QLabel* channelLabel = new QLabel(QString::number(std::get<2>(t)), this);
        channelLabel->setObjectName(deviceID + "|numChannels");
        QLabel* rangeLabel = new QLabel(QString::number(std::get<3>(t)), this);
        rangeLabel->setObjectName(deviceID + "|range");
        QLabel* handleLabel = new QLabel(QString::number(std::get<0>(t)), this);
        handleLabel->setObjectName(deviceID + "|handle");

        int row = this->ui->MCCDAQDevicesLayout->rowCount();
        this->ui->MCCDAQDevicesLayout->addWidget(chb, row, 0);
        this->ui->MCCDAQDevicesLayout->addWidget(new QLabel("Analog Input: " + QString(std::get<1>(t) ? "yes" : "no"), this), row, 1);
        this->ui->MCCDAQDevicesLayout->addWidget(new QLabel("Handle: ", this), row, 2);
        this->ui->MCCDAQDevicesLayout->addWidget(handleLabel, row, 3);
        this->ui->MCCDAQDevicesLayout->addWidget(new QLabel("Number of channels: ", this), row, 4);
        this->ui->MCCDAQDevicesLayout->addWidget(channelLabel, row, 5);
        this->ui->MCCDAQDevicesLayout->addWidget(new QLabel("Voltage range: ", this), row, 6);
        this->ui->MCCDAQDevicesLayout->addWidget(rangeLabel, row, 7);
    }
}
#endif

#ifdef WIRINGPI_AVAILABLE
void
DAQScanDialog::displayAvailableI2CDAQs()
{
    auto scanForAvailableI2CDAQs = []() -> std::vector<unsigned char> {
        std::vector<unsigned char> daqs;
        int f;
        // Explicit global namespace to prevent namespace collision with QDialog::open
        if ((f = ::open("/dev/i2c-1", O_RDWR)) >= 0) {
            for (unsigned char addr = 0; addr < 128; addr++) {
                if (ioctl(f, I2C_SLAVE, addr) > 0) {
                    daqs.push_back(addr);
                }
            }
        }
        return daqs;
    };

    for (const auto& addr: scanForAvailableI2CDAQs()) {
        // device id
        QString deviceID = QString::fromStdString("i2c:");
        QCheckBox* chb = new QCheckBox(deviceID, this);
        chb->setObjectName(deviceID);
        this->selectedDAQs.push_back(chb);
        // label for address
        QLabel* addrLabel = new QLabel(QString::number(addr), this);
        addrLabel->setObjectName(deviceID + "|addr");
        // combox box for channels
        QComboBox* cmb = new QComboBox(this);
        cmb->setObjectName(deviceID + "|numChannels");
        cmb->addItems({"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"});
        cmb->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

        int row = this->ui->SerialportDevicesLayout->rowCount();
        this->ui->I2CDAQDevicesLayout->addWidget(chb, row, 0);
        this->ui->I2CDAQDevicesLayout->addWidget(new QLabel("I2C Address: ", this), row, 1);
        this->ui->I2CDAQDevicesLayout->addWidget(addrLabel, row, 2);
        this->ui->I2CDAQDevicesLayout->addWidget(new QLabel("Number of 'channels': ", this), row, 3);
        this->ui->I2CDAQDevicesLayout->addWidget(cmb, row, 4);
    }
}
#endif
