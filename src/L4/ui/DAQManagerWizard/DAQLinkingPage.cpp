#include "DAQLinkingPage.h"

DAQLinkingPage::DAQLinkingPage(std::vector<std::string> sensorIDs, QWidget *parent) :
    QWizardPage(parent), sensorIDs{sensorIDs}
{}

void
DAQLinkingPage::initializePage()
{
    QWidget* page = new QWidget(this);
    QGridLayout* gl = new QGridLayout();

    DAQManagerWizard* dmw = (DAQManagerWizard*)this->wizard();
    QStringList options;
    for (const auto& p : dmw->abstractDAQData) {
        if (!p.second) continue;
        for (int i = 0; i < this->field(QString::fromStdString(p.first + "|numChannels")).toInt(); i++) {
            options.push_back(QString::fromStdString(p.first + "-" + std::to_string(i)));
        }
    }
    options.push_back("<unlinked>");

    gl->addWidget(new QLabel("Sensor ID", this), 0, 0);
    gl->addWidget(new QLabel("DAQ Channel", this), 0, 1);
    for (unsigned char i = 0; i < this->sensorIDs.size(); i++) {
        QComboBox* cmb = new QComboBox(this);
        cmb->addItems(options);
        gl->addWidget(new QLabel(QString::fromStdString(this->sensorIDs.at(i)), this), i+1, 0);
        gl->addWidget(cmb, i+1, 1);
    }

    page->setLayout(gl);
    this->setLayout(new QVBoxLayout());
    this->layout()->addWidget(page);
}
