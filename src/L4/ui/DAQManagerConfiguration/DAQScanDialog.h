#pragma once

#include <QDialog>
#include <QComboBox>
#include <QCheckBox>
#include <QDir>
#include "DAQManager.h"
#include "DummyDAQ.h"
#include "SerialPortDAQ.h"
#ifdef ULDAQ_AVAILABLE
#include <uldaq.h>
#include "AiMCCDAQ.h"
#endif
#ifdef WIRINGPI_AVAILABLE
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include "I2CDAQ.h"
#endif
#include "ui_DAQScanDialog.h"



class DAQScanDialog : public QDialog
{
    Q_OBJECT
public:
    DAQScanDialog(QWidget* parent = nullptr);
    std::vector<Sensor*> DAQDevices;
private:
    void accept() override;

    void displayDummyDAQs();
    void displayOpenSerialPorts();
#ifdef ULDAQ_AVAILABLE
    void displayAvailableAiMCCDAQs();
#endif
#ifdef WIRINGPI_AVAILABLE
    void displayAvailableI2CDAQs();
#endif

    std::vector<QCheckBox*> selectedDAQs;
    Ui::DAQScanDialog ui;
};
