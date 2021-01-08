#pragma once

#ifdef ULDAQ_AVAILABLE
    #include <uldaq.h>
#endif
#include "easylogging++.h"
#include "IDAQDeviceHandler.h"
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
    static std::unique_ptr<DAQManager> createDAQManager();
    ~DAQManagerFactory();
protected:
#ifdef ULDAQ_AVAILABLE
    std::map<std::string, AiDAQInfo> selectedAiMccdaqs;
#endif
    std::map<std::string, SerialPortInfo> selectedSerialports;
private:
    explicit DAQManagerFactory(QWidget *parent = nullptr);
    Ui::DAQManagerFactory *ui;
private slots:
#ifdef ULDAQ_AVAILABLE
    void scanForAiMCCDAQs();
#endif
    void openAndTestSerialPort();
};
