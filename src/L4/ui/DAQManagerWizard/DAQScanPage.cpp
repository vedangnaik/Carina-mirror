#include "DAQScanPage.h"

DAQScanPage::DAQScanPage(QWidget* parent)
    : QWizardPage(parent), ui(new Ui::DAQManagerFactory)
{
    ui->setupUi(this);
}

void
DAQScanPage::initializePage()
{
<<<<<<< HEAD
   this->displayDummyDAQs();
   this->displayOpenSerialPorts();
#ifdef ULDAQ_AVAILABLE
   this->displayAvailableAiMCCDAQs();
=======
    this->displayDummyDAQs();
    this->displayOpenSerialPorts();
#ifdef ULDAQ_AVAILABLE
    this->displayAvailableAiMCCDAQs();
>>>>>>> 50e4af9f13645a4aa7b9738c929657a89b9df54c
#endif
#ifdef WIRINGPI_AVAILABLE
    this->displayAvailableI2CDAQs();
#endif
}

void
DAQScanPage::displayDummyDAQs()
{
    DAQManagerWizard* dmw = (DAQManagerWizard*)this->wizard();

    // create device ID and insert into the map as false
    std::string deviceID = "dummy:0";
    dmw->abstractDAQData.insert({ deviceID, false });

    // create channels combo box here and register it
    QComboBox* cmb = new QComboBox(this);
    cmb->addItems({"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"});
    cmb->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    this->registerField(QString::fromStdString(deviceID + "|numChannels"), cmb);

    // create the checkbox and connect it
    QCheckBox* d = new QCheckBox(QString::fromStdString(deviceID), this);
    connect(d, &QCheckBox::stateChanged, this, [=](int state) {
        state == Qt::Unchecked ? dmw->abstractDAQData.at(deviceID) = false : dmw->abstractDAQData.at(deviceID) = true;
    });

    // display on UI
    int row = this->ui->MCCDAQDevicesLayout->rowCount();
    this->ui->DummyDAQsLayout->addWidget(d, row, 0);
    this->ui->DummyDAQsLayout->addWidget(new QLabel("Number of channels: ", this), row, 1);
    this->ui->DummyDAQsLayout->addWidget(cmb, row, 2);
}

void
DAQScanPage::displayOpenSerialPorts()
{
    DAQManagerWizard* dmw = (DAQManagerWizard*)this->wizard();

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
        std::string deviceID = "serialport:" + std::get<0>(t);
        dmw->abstractDAQData.insert({ deviceID, false });

        QCheckBox* chb = new QCheckBox(QString::fromStdString(deviceID), this);
        connect(chb, &QCheckBox::stateChanged, this, [=](int state) {
            state == Qt::Unchecked ? dmw->abstractDAQData.at(deviceID) = false : dmw->abstractDAQData.at(deviceID) = true;
        });

        QComboBox* cmb = new QComboBox(this);
        cmb->addItems({"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"});
        cmb->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        this->registerField(QString::fromStdString(deviceID + "|numChannels"), cmb);

        QLabel* l = new QLabel(QString::fromStdString(std::get<1>(t)), this);
        this->registerField(QString::fromStdString(deviceID + "|serialportPath"), l, "text");

        int row = this->ui->SerialportDevicesLayout->rowCount();
        this->ui->SerialportDevicesLayout->addWidget(chb, row, 0);
        this->ui->SerialportDevicesLayout->addWidget(new QLabel("Path: ", this), row, 1);
        this->ui->SerialportDevicesLayout->addWidget(l, row, 2);
        this->ui->SerialportDevicesLayout->addWidget(new QLabel("Number of 'channels': ", this), row, 3);
        this->ui->SerialportDevicesLayout->addWidget(cmb, row, 4);
    }
}

/* UNTESTED, PLEASE BE WARY
*/
#ifdef ULDAQ_AVAILABLE
void
DAQScanPage::displayAvailableAiMCCDAQs()
{
    DAQManagerWizard* dmw = (DAQManagerWizard*)this->wizard();

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
            err = ulAIGetInfo(handle, AI_INFO_NUM_CHANS, 0, &numChannels);
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
        std::string deviceID = "mccdaq:" + std::to_string(std::get<0>(t));
        dmw->abstractDAQData.insert({ deviceID, false });

        QCheckBox* chb = new QCheckBox(QString::fromStdString(deviceID), this);
        connect(chb, &QCheckBox::stateChanged, this, [=](int state) {
            state == Qt::Unchecked ? dmw->abstractDAQData.at(deviceID) = false : dmw->abstractDAQData.at(deviceID) = true;
        });

        QLabel* l = new QLabel(QString::number(std::get<2>(t)), this);
        this->registerField(QString::fromStdString(deviceID + "|numChannels"), l, "text");

        QLabel* m = new QLabel(QString::number(std::get<3>(t)), this);
        this->registerField(QString::fromStdString(deviceID + "|range"), l, "text");

        QLabel* n = new QLabel(QString::number(std::get<0>(t)), this);
        this->registerField(QString::fromStdString(deviceID + "|handle"), l, "text");

        int row = this->ui->MCCDAQDevicesLayout->rowCount();
        this->ui->MCCDAQDevicesLayout->addWidget(chb, row, 0);
        this->ui->MCCDAQDevicesLayout->addWidget(new QLabel("Analog Input: " + QString(std::get<1>(t) ? "yes" : "no"), this), row, 1);
        this->ui->MCCDAQDevicesLayout->addWidget(new QLabel("Handle: ", this), row, 2);
        this->ui->MCCDAQDevicesLayout->addWidget(n, row, 3);
        this->ui->MCCDAQDevicesLayout->addWidget(new QLabel("Number of channels: ", this), row, 4);
        this->ui->MCCDAQDevicesLayout->addWidget(l, row, 5);
        this->ui->MCCDAQDevicesLayout->addWidget(new QLabel("Voltage range: ", this), row, 6);
        this->ui->MCCDAQDevicesLayout->addWidget(m, row, 7);
    }
}
#endif

#ifdef WIRINGPI_AVAILABLE
void
DAQScanPage::displayAvailableI2CDAQs()
{
    auto scanForAvailableI2CDAQs = []() -> std::vector<unsigned char> {
            std::vector<unsigned char> daqs;
        int f;
        if ((f = open("/dev/i2c-1", O_RDWR)) >= 0) {
            for (unsigned char addr = 0; addr < 128; addr++) {
                if (ioctl(f, I2C_SLAVE, addr) > 0) {
                    daqs.push_back(addr);
                }
            }
        }
        return daqs;
    };

    DAQManagerWizard* dmw = (DAQManagerWizard*)this->wizard();
    for (const auto& addr: scanForAvailableI2CDAQs()) {
        std::string deviceID = "i2c:" + std::to_string(addr);
        dmw->abstractDAQData.insert({ deviceID, false });

        QCheckBox* chb = new QCheckBox(QString::fromStdString(deviceID), this);
        connect(chb, &QCheckBox::stateChanged, this, [=](int state) {
            state == Qt::Unchecked ? dmw->abstractDAQData.at(deviceID) = false : dmw->abstractDAQData.at(deviceID) = true;
        });

        QLabel* l = new QLabel(QString::number(addr), this);
        this->registerField(QString::fromStdString(deviceID + "|i2cAddr"), l, "text");

        QComboBox* cmb = new QComboBox(this);
        cmb->addItems({"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"});
        cmb->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        this->registerField(QString::fromStdString(deviceID + "|numChannels"), cmb);

        int row = this->ui->SerialportDevicesLayout->rowCount();
        this->ui->I2CDAQDevicesLayout->addWidget(chb, row, 0);
        this->ui->I2CDAQDevicesLayout->addWidget(new QLabel("I2C Address: ", this), row, 1);
        this->ui->I2CDAQDevicesLayout->addWidget(l, row, 2);
        this->ui->I2CDAQDevicesLayout->addWidget(new QLabel("Number of 'channels': ", this), row, 3);
        this->ui->I2CDAQDevicesLayout->addWidget(cmb, row, 4);
    }
}
#endif
