#include "SensorToDAQLinker.h"
#include "ui_SensorToDAQLinker.h"

SensorToDAQLinker::SensorToDAQLinker(std::vector<std::string> sensorIDs, std::vector<AbstractDAQ*> DAQDevices, QWidget *parent) : QDialog(parent), ui(new Ui::SensorToDAQLinker), DAQDevices{DAQDevices} {
    ui->setupUi(this);

    std::map<std::string, AbstractDAQ*> DAQDeviceIDs;
    for (const auto& d : DAQDevices) {
        DAQDeviceIDs.insert({ d->deviceID, d });
    }

    for (const auto& id : sensorIDs) {
        QComboBox* cmb = new QComboBox(this);
        cmb->addItem("<unlinked>");
        for (const auto& d : DAQDevices) {
            for (unsigned int i = 0; i < d->numChannels; i++) {
                cmb->addItem(QString::fromStdString(d->deviceID + "-" + std::to_string(i)));
            }
        }
        this->ui->sensorAndDaqFormLayout->addRow(QString::fromStdString(id), cmb);
    }

    connect(ui->closeButton, &QPushButton::clicked, this, &SensorToDAQLinker::reject);
    connect(ui->linkButton, &QPushButton::clicked, this, &SensorToDAQLinker::accept);
}

SensorToDAQLinker::~SensorToDAQLinker() {
    delete ui;
}

void SensorToDAQLinker::accept() {
    for (int i = 0; i < this->ui->sensorAndDaqFormLayout->rowCount(); i++) {
        std::string sensorID = ((QLabel*)this->ui->sensorAndDaqFormLayout->itemAt(i, QFormLayout::ItemRole::LabelRole)->widget())->text().toStdString();
        QString choice = ((QComboBox*)this->ui->sensorAndDaqFormLayout->itemAt(i, QFormLayout::ItemRole::FieldRole)->widget())->currentText();
        if (choice == "<unlinked>") {
            // Pass, this sensor doesn't have a linkage.
            continue;
        } else {
            QStringList parts = choice.split("-");
            std::string deviceID = parts.at(0).toStdString();
            // Find the AbstractDAQ* that this deviceID points to. Guaranteed to not be .end();
            auto adhIt = std::find_if(this->DAQDevices.begin(), this->DAQDevices.end(), [deviceID](const auto& n) {
                return n->deviceID == deviceID;
            });
            unsigned int channel = parts.at(1).toUInt();
            this->sensorToDAQLinks.insert({ sensorID, { *adhIt, channel } });
        }
    }

    this->done(QDialog::Accepted);
}

std::map<std::string, std::pair<AbstractDAQ*, unsigned int>>
    SensorToDAQLinker::getSensorToDAQLinks(std::vector<std::string> sensorIDs, std::vector<AbstractDAQ*> DAQDevices)
{
    SensorToDAQLinker* stdl = new SensorToDAQLinker(sensorIDs, DAQDevices);
    if (stdl->exec() == QDialog::Accepted) {
        return stdl->sensorToDAQLinks;
    } else {
        return {};
    }
}
