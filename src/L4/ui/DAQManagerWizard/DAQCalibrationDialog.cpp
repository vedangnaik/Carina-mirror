#include "DAQCalibrationDialog.h"

DAQCalibrationDialog::DAQCalibrationDialog(std::unique_ptr<DAQManager> daqm, QWidget *parent)
    : QDialog(parent), daqm{std::move(daqm)}
{
    QWidget* scrollAreaWidget = new QWidget(this);
    QVBoxLayout* vb = new QVBoxLayout();
    vb->addStretch();
    scrollAreaWidget->setLayout(vb);

    for (const auto& daq : this->daqm->DAQDevices) {
        QGroupBox* gb = new QGroupBox(QString::fromStdString(daq->deviceID), this);
        QFormLayout* daqFormLayout = new QFormLayout(this);
        daqFormLayout->setObjectName(QString::fromStdString(daq->deviceID));
        gb->setLayout(daqFormLayout);

        for (unsigned int channel = 0; channel < daq->numChannels; channel++) {
            auto v = new QDoubleValidator(this);
            daqFormLayout->addRow("Channel " + QString::number(channel) + ": Raw Voltage", new QLabel("Calibrated Value", this));
            for (size_t i = 0; i < daq->calibrationPoints.at(0).size(); i++) {
                // Raw voltage box
                auto rawVoltage = new QLineEdit(this);
                rawVoltage->setValidator(v);
                rawVoltage->setText(QString::number(daq->calibrationPoints.at(channel).at(i).first));
                // Calibrated value box
                auto calibratedValue = new QLineEdit(this);
                calibratedValue->setValidator(v);
                calibratedValue->setText(QString::number(daq->calibrationPoints.at(channel).at(i).second));

                daqFormLayout->addRow(rawVoltage, calibratedValue);
            }
        }

        scrollAreaWidget->layout()->addWidget(gb);
    }

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
DAQCalibrationDialog::accept()
{
    for (const auto& daq : this->daqm->DAQDevices) {
        QFormLayout* daqFormLayout = this->findChild<QFormLayout*>(QString::fromStdString(daq->deviceID));
        unsigned int numCalibrationPoints = daq->calibrationPoints.at(0).size();
        for (unsigned int channel = 0; channel < daq->numChannels; channel++) {
            int startRow = (1 + numCalibrationPoints) * channel + 1;
            for (size_t row = startRow, i = 0; row < startRow + numCalibrationPoints; row++, i++) {
                // Utterly hideous, dear god
                daq->calibrationPoints.at(channel).at(i).first = ((QLineEdit*)daqFormLayout->itemAt(row, QFormLayout::LabelRole)->widget())->text().toDouble();
                daq->calibrationPoints.at(channel).at(i).second = ((QLineEdit*)daqFormLayout->itemAt(row, QFormLayout::FieldRole)->widget())->text().toDouble();
            }
        }
    }
    this->done(QDialog::Accepted);
}

std::unique_ptr<DAQManager>
DAQCalibrationDialog::takeDAQManager()
{
    return std::move(this->daqm);
}
