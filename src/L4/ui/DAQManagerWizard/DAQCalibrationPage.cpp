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
