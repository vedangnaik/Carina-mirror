#pragma once

#include <QDialog>
#include <QComboBox>
#include <QCheckBox>
#include <QDir>
#include "DAQManagerWizard.h"
#ifdef ULDAQ_AVAILABLE
#include "uldaq.h"
#endif
#ifdef WIRINGPI_AVAILABLE
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#endif
#include "ui_DAQScanPage.h"



class DAQScanDialog : public QDialog
{
    Q_OBJECT
public:
    DAQScanDialog(QWidget* parent = nullptr);
    std::vector<AbstractDAQ*> DAQDevices;
private:
    void accept() override;

    void displayDummyDAQs();
//    void displayOpenSerialPorts();
#ifdef ULDAQ_AVAILABLE
//    void displayAvailableAiMCCDAQs();
#endif
#ifdef WIRINGPI_AVAILABLE
    void displayAvailableI2CDAQs();
#endif

    std::vector<QCheckBox*> selectedDAQs;
    Ui::DAQScanPage* ui;
};
