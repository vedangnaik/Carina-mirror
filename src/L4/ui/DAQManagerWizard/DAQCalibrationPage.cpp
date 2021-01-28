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
        QGridLayout* gl = new QGridLayout();
        gb->setLayout(gl);

        this->calibrationPoints.insert({ p.first, std::make_pair(std::vector<double>(5, 1), std::vector<double>(5, 1)) });

        auto v = new QDoubleValidator(this);
        gl->addWidget(new QLabel("Raw Voltage", this), 0, 0);
        gl->addWidget(new QLabel("Calibrated Value", this), 0, 1);
        for (int i = 1; i < 6; i++) {
            auto rawVoltage = new QLineEdit(this);
            auto calibratedValue = new QLineEdit(this);
            rawVoltage->setValidator(v);
            calibratedValue->setValidator(v);
            gl->addWidget(rawVoltage, i, 0);
            gl->addWidget(calibratedValue, i, 1);

            connect(rawVoltage, &QLineEdit::textEdited, this, [=](const QString& text) {
                this->calibrationPoints.at(p.first).first.at(i-1) = text.toDouble();
            });
            connect(calibratedValue, &QLineEdit::textEdited, this, [=](const QString& text) {
                this->calibrationPoints.at(p.first).second.at(i-1) = text.toDouble();
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
}
