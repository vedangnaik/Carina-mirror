#include "DAQLinkingPage.h"

DAQLinkingPage::DAQLinkingPage(std::vector<std::string> sensorIDs, QWidget *parent) :
    QWizardPage(parent), sensorIDs{sensorIDs}
{}

void
DAQLinkingPage::initializePage()
{
    QWidget* page = new QWidget(this);
    QFormLayout* fl = new QFormLayout();

    DAQManagerWizard* dmw = (DAQManagerWizard*)this->wizard();
    QStringList options;
    for (const auto& p : dmw->abstractDAQData) {
        if (!p.second) continue;
        for (int i = 0; i <= this->field(QString::fromStdString(p.first + "|numChannels")).toInt(); i++) {
            options.push_back(QString::fromStdString(p.first + "-" + std::to_string(i)));
        }
    }
    options.push_back("<unlinked>");

    fl->addRow(new QLabel("Sensor ID", this), new QLabel("DAQ Channel", this));
    for (const auto& id : this->sensorIDs) {
        QComboBox* cmb = new QComboBox(this);
        connect(cmb, &QComboBox::currentTextChanged, this, [=](const QString& text) {
            // only place where operator[] is useful xD, and only cause
            // we don't have insert_or_assign() from cpp17 :(
            this->sensorLinks[id] = text.toStdString();
        });
        cmb->addItems(options);

        if(this->sensorLinks.find(id) != this->sensorLinks.end()) {
            cmb->setCurrentText(QString::fromStdString(this->sensorLinks.at(id)));
        }

        fl->addRow(new QLabel(QString::fromStdString(id), this), cmb);
    }

    page->setLayout(fl);
    this->setLayout(new QVBoxLayout());
    this->layout()->addWidget(page);
}

void
DAQLinkingPage::cleanupPage()
{
    uihelpers::clearLayout(this->layout());
    delete this->layout();
}
