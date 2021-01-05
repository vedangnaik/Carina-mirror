#pragma once

#ifdef ULDAQ_AVAILABLE
    #include <uldaq.h>
#endif
#include "easylogging++.h"
#include "DAQDeviceHandler.h"
#include "DAQManager.h"
#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QDir>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include "ui_daqmanagerfactory.h"

namespace Ui {
    class DAQManagerFactory;
}

class DAQManagerFactory : public QDialog {
    Q_OBJECT
public:
    explicit DAQManagerFactory(QWidget *parent = nullptr);
    ~DAQManagerFactory();

    static std::unique_ptr<DAQManager> createDAQManager();
private slots:
#ifdef ULDAQ_AVAILABLE
    void scanForMCCDAQs();
#endif
    void openAndTestSerialPort();
private:
    QSpinBox* getSerialPortChannelsSpinBox();
    std::map<std::string, bool> detectedMccdaqs;
    std::map<std::string, bool> detectedSerialports;
    Ui::DAQManagerFactory *ui;
};
