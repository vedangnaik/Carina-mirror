#include "DAQManagerWizard.h"

DAQManagerWizard::DAQManagerWizard(QWidget* parent)
    : QWizard(parent)
{
    this->addPage(new DataSourceAcquisitionPage());
    this->addPage(new DummyLastPage());
}

void
DAQManagerWizard::accept()
{
    QDialog::accept();
}

//===========================================
DataSourceAcquisitionPage::DataSourceAcquisitionPage(QWidget* parent)
    : QWizardPage(parent), ui(new Ui::DAQManagerFactory)
{
    ui->setupUi(this);
}

void
DataSourceAcquisitionPage::initializePage()
{
    // add dummy DAQ id to list in wizard
    std::string id = "dummy:0";
    DAQManagerWizard* dmw = (DAQManagerWizard*)this->wizard();
    dmw->abstractDAQData.insert({ id, false });

    // Create required fields here and connect checkbox
    QComboBox* cmb = new QComboBox(this);
    cmb->addItems({"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"});
    cmb->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    QCheckBox* d = new QCheckBox(QString::fromStdString(id), this);
    connect(d, &QCheckBox::stateChanged, this, [=](int state) {
        DAQManagerWizard* dmw = (DAQManagerWizard*)this->wizard();
        state == Qt::Unchecked ? dmw->abstractDAQData.at(id) = false : dmw->abstractDAQData.at(id) = true;
    });
    this->registerField(QString::fromStdString(id + "|numChannels"), cmb);

    // display on UI.
    int row = this->ui->MCCDAQDevicesLayout->rowCount();
    this->ui->DummyDAQsLayout->addWidget(d, row, 0);
    this->ui->DummyDAQsLayout->addWidget(new QLabel("Number of channels: ", this), row, 1);
    this->ui->DummyDAQsLayout->addWidget(cmb, row, 2);
}

//===========================================
DummyLastPage::DummyLastPage(QWidget* parent)
    : QWizardPage(parent)
{
    this->l = new QLabel(this);
    QHBoxLayout* hl = new QHBoxLayout();
    hl->addWidget(this->l);
    delete this->layout();
    this->setLayout(hl);
}

void
DummyLastPage::initializePage()
{
    DAQManagerWizard* dmw = (DAQManagerWizard*)this->wizard();
    for (const auto& p : dmw->abstractDAQData) {
        LOG(INFO) << p.first << ": " << p.second;
    }
}
