#include "SensorToDAQLinker.h"
#include "ui_SensorToDAQLinker.h"

SensorToDAQLinker::SensorToDAQLinker(std::vector<std::string> sensorIDs, std::vector<AbstractDAQDeviceHandler*> DAQDevices, QWidget *parent) : QDialog(parent), ui(new Ui::SensorToDAQLinker) {
    ui->setupUi(this);

    std::map<std::string, AbstractDAQDeviceHandler*> DAQDeviceIDs;
    for (const auto& d : DAQDevices) {
        DAQDeviceIDs.insert({ d->deviceID, d });
    }

    for (const auto& id : sensorIDs) {
        QComboBox* cmb = new QComboBox(this);
        for (const auto& d : DAQDevices) {
            for (unsigned int i = 0; i < d->numChannels; i++) {
                cmb->addItem(QString::fromStdString(d->deviceID + "-" + std::to_string(i)));
            }
        }

        connect(cmb, &QComboBox::currentTextChanged, this, [=](const QString& t) {
            QStringList parts = t.split("-");
            this->sensorToDAQLinks.insert({ id, { DAQDeviceIDs.at(parts.at(0).toStdString()), parts.at(1).toUInt() } });
        });

        this->ui->sensorAndDaqFormLayout->addRow(QString::fromStdString(id), cmb);
    }

    connect(ui->closeButton, &QPushButton::clicked, this, &SensorToDAQLinker::reject);
    connect(ui->linkButton, &QPushButton::clicked, this, &SensorToDAQLinker::accept);
}

SensorToDAQLinker::~SensorToDAQLinker() {
    delete ui;
}

std::map<std::string, std::pair<AbstractDAQDeviceHandler*, unsigned int>> SensorToDAQLinker::getSensorToDAQLinks(std::vector<std::string> sensorIDs, std::vector<AbstractDAQDeviceHandler*> DAQDevices) {
    SensorToDAQLinker* stdl = new SensorToDAQLinker(sensorIDs, DAQDevices);
    int r = stdl->exec();
    if (r == QDialog::Accepted) {
        return stdl->sensorToDAQLinks;
    } else {
        return {};
    }
}
