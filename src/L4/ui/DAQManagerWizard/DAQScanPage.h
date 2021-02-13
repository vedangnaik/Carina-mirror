#pragma once

#include <QWizardPage>
#include <QComboBox>
#include <QCheckBox>
#include <QDir>
#include "DAQManagerWizard.h"
#include "ui_DAQScanPage.h"
#ifdef ULDAQ_AVAILABLE
#include "uldaq.h"
#endif

class DAQScanPage : public QWizardPage
{
    Q_OBJECT
public:
    DAQScanPage(QWidget* parent = nullptr);
private:
    void initializePage() override;
    void displayDummyDAQs();
    void displayOpenSerialPorts();
#ifdef ULDAQ_AVAILABLE
    void displayAvailableAiMCCDAQs();
#endif
    Ui::DAQManagerFactory* ui;
};
