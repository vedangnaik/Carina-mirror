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

#ifdef WIRINGPI_AVAILABLE
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
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
#ifdef WIRINGPI_AVAILABLE
    void displayAvailableI2CDAQs();
#endif
    Ui::DAQManagerFactory* ui;
};
