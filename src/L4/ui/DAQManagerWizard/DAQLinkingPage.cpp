#include "DAQLinkingPage.h"

DAQLinkingPage::DAQLinkingPage(std::vector<std::string> sensorIDs, QWidget *parent) :
    QWizardPage(parent)
{
    for (const auto& id : sensorIDs) {
        this->sensorLinks.insert({ id, {} });
    }
}

void
DAQLinkingPage::initializePage()
{
    QWidget* page = new QWidget(this);
    QGridLayout* gl = new QGridLayout();

    DAQManagerWizard* dmw = (DAQManagerWizard*)this->wizard();
    QStringList options;
    for (const auto& p : dmw->abstractDAQData) {
        if (!p.second) continue;
        for (int i = 0; i <= this->field(QString::fromStdString(p.first + "|numChannels")).toInt(); i++) {
            options.push_back(QString::fromStdString(p.first + "-" + std::to_string(i)));
        }
    }
    options.push_back("<unlinked>");

    int row = 0;
    gl->addWidget(new QLabel("Sensor ID", this), row, 0);
    gl->addWidget(new QLabel("DAQ Channel", this), row++, 1);
    for (const auto& p : this->sensorLinks) {
        QComboBox* cmb = new QComboBox(this);
        connect(cmb, &QComboBox::currentTextChanged, this, [=](const QString& text) {
            this->sensorLinks.at(p.first) = text.toStdString();
        });

        cmb->addItems(options);
        gl->addWidget(new QLabel(QString::fromStdString(p.first), this), row, 0);
        gl->addWidget(cmb, row++, 1);
    }

    page->setLayout(gl);
    this->setLayout(new QVBoxLayout());
    this->layout()->addWidget(page);
}

void
DAQLinkingPage::cleanupPage()
{
    uihelpers::clearLayout(this->layout());
    delete this->layout();
}
