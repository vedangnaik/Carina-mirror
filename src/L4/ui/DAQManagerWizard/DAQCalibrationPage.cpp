#include "DAQCalibrationPage.h"

DAQCalibrationPage::DAQCalibrationPage(QWidget *parent) :
    QWizardPage(parent)
{}

void
DAQCalibrationPage::initializePage()
{
    QWidget* page = new QWidget(this);
    QVBoxLayout* vb = new QVBoxLayout();
    page->setLayout(vb);

    DAQManagerWizard* dmw = (DAQManagerWizard*)this->wizard();
    for (const auto& p : dmw->abstractDAQData) {
        if (!p.second) continue;

        QGroupBox* gb = new QGroupBox(QString::fromStdString(p.first), this);
        QFormLayout* fl = new QFormLayout();
        gb->setLayout(fl);

        bool present = this->calibrationPoints.find(p.first) != this->calibrationPoints.end();
        if (!present) {
            this->calibrationPoints.insert({ p.first, { std::array<double, 5>{1, 2, 3, 4, 5}, std::array<double, 5>{1, 2, 3, 4, 5} }});
        }

        auto v = new QDoubleValidator(this);
        fl->addRow(new QLabel("Raw Voltage", this), new QLabel("Calibrated Value", this));
        for (int i = 0; i < 5; i++) {
            auto rawVoltage = new QLineEdit(this);
            auto calibratedValue = new QLineEdit(this);
            rawVoltage->setValidator(v);
            calibratedValue->setValidator(v);
            rawVoltage->setText(QString::number(this->calibrationPoints.at(p.first).first.at(i)));
            calibratedValue->setText(QString::number(this->calibrationPoints.at(p.first).second.at(i)));
            fl->addRow(rawVoltage, calibratedValue);

            connect(rawVoltage, &QLineEdit::textEdited, this, [=](const QString& text) {
                this->calibrationPoints.at(p.first).first.at(i) = text.toDouble();
            });
            connect(calibratedValue, &QLineEdit::textEdited, this, [=](const QString& text) {
                this->calibrationPoints.at(p.first).second.at(i) = text.toDouble();
            });
        }

        page->layout()->addWidget(gb);
    }

    vb->addStretch();
    QScrollArea* sa = new QScrollArea(this);
    sa->setFrameStyle(QFrame::NoFrame);
    sa->setWidget(page);
    sa->setWidgetResizable(true);
    this->setLayout(new QVBoxLayout());
    this->layout()->addWidget(sa);
}

void
DAQCalibrationPage::cleanupPage()
{
    uihelpers::clearLayout(this->layout());
    delete this->layout();
}
