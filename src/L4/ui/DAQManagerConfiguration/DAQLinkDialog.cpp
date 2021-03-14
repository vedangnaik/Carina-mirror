#include "DAQLinkDialog.h"

DAQLinkDialog::DAQLinkDialog(std::unique_ptr<DAQManager> daqm, QWidget *parent)
    : QDialog(parent), daqm{std::move(daqm)}
{
    // Get all channels from all daqs
    QStringList options{"<unlinked>"};
    for (const auto& daq : this->daqm->DAQDevices) {
        for (unsigned int channel = 0; channel < daq->numChannels; channel++) {
            options.push_back(QString::fromStdString(daq->deviceID + "-") + QString::number(channel));
        }
    }

    // Create a combo box with all channels, then set it to the current one
    QFormLayout* sensorLinksFormLayout = new QFormLayout();
    sensorLinksFormLayout->setObjectName("fl");
    for (const auto& sensorID : this->daqm->getSensorIDs()) {
        QComboBox* cmb = new QComboBox(this);
        cmb->addItems(options);
        sensorLinksFormLayout->addRow(QString::fromStdString(sensorID), cmb);

        if (this->daqm->sensorToDAQLinks.find(sensorID) != this->daqm->sensorToDAQLinks.end()) {
            AbstractDAQ* daq = this->daqm->sensorToDAQLinks.at(sensorID).first;
            unsigned int channel = this->daqm->sensorToDAQLinks.at(sensorID).second;
            cmb->setCurrentText(QString::fromStdString(daq->deviceID + "-") + QString::number(channel));
        }
    }

    QWidget* scrollAreaWidget = new QWidget(this);
    scrollAreaWidget->setLayout(sensorLinksFormLayout);
    QScrollArea* sa = new QScrollArea(this);
    sa->setFrameStyle(QFrame::NoFrame);
    sa->setWidget(scrollAreaWidget);
    sa->setWidgetResizable(true);
    this->setLayout(new QVBoxLayout());
    this->layout()->addWidget(sa);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    this->layout()->addWidget(buttonBox);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void
DAQLinkDialog::accept()
{
    this->daqm->sensorToDAQLinks.clear();
    QFormLayout* fl = this->findChild<QFormLayout*>("fl");
    for (int i = 0; i < fl->rowCount(); i++) {
        std::string sensorID = ((QLabel*)fl->itemAt(i, QFormLayout::LabelRole)->widget())->text().toStdString();
        QString daqAndChannel = ((QComboBox*)fl->itemAt(i, QFormLayout::FieldRole)->widget())->currentText();

        if (daqAndChannel == "<unlinked>") { continue; }

        const auto p = daqAndChannel.split("-");
        std::string deviceID = p.at(0).toStdString();
        unsigned int channel = p.at(1).toUInt();
        AbstractDAQ* daq = *std::find_if(this->daqm->DAQDevices.begin(), this->daqm->DAQDevices.end(), [=](AbstractDAQ* d) {
            return d->deviceID == deviceID;
        });

        this->daqm->sensorToDAQLinks[sensorID] = std::make_pair(daq, channel);
    }

    this->done(QDialog::Accepted);
}

std::unique_ptr<DAQManager>
DAQLinkDialog::takeDAQManager()
{
    return std::move(this->daqm);
}

