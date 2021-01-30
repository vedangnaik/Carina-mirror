#include "DAQScanPage.h"

DAQScanPage::DAQScanPage(QWidget* parent)
    : QWizardPage(parent), ui(new Ui::DAQManagerFactory)
{
    ui->setupUi(this);
}

void
DAQScanPage::initializePage()
{
   this->displayDummyDAQs();
   this->displayOpenSerialPorts();
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
//            if (test.is_open()) {
                ports.push_back(std::make_tuple(filename, path));
//            }
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
        this->ui->SerialportDevicesLayout->addWidget(new QLabel("\tNumber of 'channels': ", this), row, 3);
        this->ui->SerialportDevicesLayout->addWidget(cmb, row, 4);
    }
}
