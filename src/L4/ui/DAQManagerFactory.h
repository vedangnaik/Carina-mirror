#pragma once

#ifdef ULDAQ_AVAILABLE
    #include <uldaq.h>
#endif
#include "easylogging++.h"
#include "DAQManager.h"
#include "SerialPortDAQ.h"
#include "AiMCCDAQ.h"
#include "DummyDAQ.h"
#include "ui_DAQManagerFactory.h"

#include <tuple>
#include <QDialog>
#include <QLabel>
#include <QDir>
#include <QComboBox>
#include <QCheckBox>
#include <unistd.h>

namespace Ui {
    class DAQManagerFactory;
}

class DAQManagerFactory : public QDialog {
    Q_OBJECT
public:
    static std::unique_ptr<DAQManager> createDAQManager();
private:
    DAQManagerFactory(QWidget *parent = nullptr);
    ~DAQManagerFactory();
    std::vector<AbstractDAQ*> prospectiveDAQDevices;
    Ui::DAQManagerFactory *ui;
private slots:
    void accept() override;
};
