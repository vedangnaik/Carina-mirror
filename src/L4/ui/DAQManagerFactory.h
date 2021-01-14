#pragma once

#ifdef ULDAQ_AVAILABLE
    #include <uldaq.h>
#endif
#include "easylogging++.h"
#include "AbstractDAQDeviceHandler.h"
#include "DAQManager.h"
#include "SerialPortHandler.h"
#include "AiDAQHandler.h"
#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QDir>
#include <QComboBox>
#include <QCheckBox>
#include <QGridLayout>
#include <QLayoutItem>
#include "ui_daqmanagerfactory.h"

#ifdef ULDAQ_AVAILABLE
struct AiDAQInfo {
    std::string id;
    DaqDeviceHandle handle;
    unsigned int numChannels;
    Range voltageRange;
};
#endif

struct SerialPortInfo {
    std::string id;
    std::string serialportPath;
    unsigned int numChannels;
};

namespace Ui {
    class DAQManagerFactory;
}

class DAQManagerFactory : public QDialog {
    Q_OBJECT
public:
    DAQManagerFactory(QWidget *parent = nullptr);
    ~DAQManagerFactory();
    static std::unique_ptr<DAQManager> createDAQManager();
public slots:
    void accept() override;
    void reject() override;

#ifdef ULDAQ_AVAILABLE
private:
    std::map<std::string, AiDAQInfo> selectedAiMccdaqs;
private slots:
    void openAndTestAiMCCDAQs();
#endif

private:
    std::map<std::string, SerialPortInfo> selectedSerialports;
    std::vector<AbstractDAQDeviceHandler*> prospectiveDAQDevices;
    Ui::DAQManagerFactory *ui;
private slots:
    void openAndTestSerialPort();
};
