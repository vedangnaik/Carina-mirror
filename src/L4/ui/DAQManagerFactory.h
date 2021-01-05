#pragma once

#ifdef ULDAQ_AVAILABLE
    #include <uldaq.h>
#endif
#include "easylogging++.h"
#include <QWidget>
#include <DAQDeviceHandler.h>
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

class DAQManagerFactory : public QWidget {
    Q_OBJECT
public:
    explicit DAQManagerFactory(QWidget *parent = nullptr);
    ~DAQManagerFactory();
private slots:
#ifdef ULDAQ_AVAILABLE
    void scanForMCCDAQs();
    void openAndTestSerialPort();
#endif
private:
    QSpinBox* getSerialPortChannelsSpinBox();
    std::map<std::string, DAQDeviceHandler*> DAQs;
    Ui::DAQManagerFactory *ui;
};
